/**
  ******************************************************************************
  * @file    anemometer.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Main program file.
  ******************************************************************************
  * @attention
  *
  * This file is part of Ultrasonic_Anemometer.
  *
  * Ultrasonic_Anemometer is free software: you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * Ultrasonic_Anemometer is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with Ultrasonic_Anemometer.  If not, see <http://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#include "anemometer.h"

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <util/delay.h>

#include "ads7822.h"
#include "calibrationEEPROM.h"
#include "lcd.h"
#include "signalProcessing.h"

// Flag indicating end of data collection.
bool dataCollected = false;

// Collected data.
uint16_t samplesTmp[SAMPLE_NUMBER];

/**
  * @brief  Generator and multiplexer initialization.
  * @retval Void.
  */
void timerGeneratorInit(void);

/**
  * @brief  Status LED initialization.
  * @retval Void.
  */
void timerLedInit(void);

/**
  * @brief  Calibration button initialization.
  * @retval Void.
  */
void timerCalibrationInit(void);

/**
  * @brief  Initialization.
  * @retval Void.
  */
void init(void);

/**
  * @brief  Starts ultrasound generator.
  * @retval Void.
  */
void startGenerator(void);

/**
  * @brief  Saves first signal.
  * @param  samples Pointer to a Samples structure.
  * @retval Void.
  */
void saveFirstDataset(Samples *samples);

/**
  * @brief  Saves second signal and partly processes data.
  * @param  samples Pointer to a Samples structure.
  * @retval Void.
  */
void saveSecondDatasetAndProcessData(Samples *samples);

/**
  * @brief  Does the rest of data processing and calculates wind value.
  * @param  samples Pointer to a Samples structure.
  * @retval Void.
  */
void continueProcessData(Samples *samples);

/**
  * @brief  Calculates signal time of flight without wind.
  * @param  temperature Measured temperature.
  * @retval Calculated time of flight.
  */
inline uint64_t calcTimeWithoutWind(int8_t temperature);

/**
  * @brief  Main function.
  * @retval Should not return.
  */
int main(void) {
  init();

  Samples samples = {
    .samplesNumber = SAMPLE_NUMBER
  };

  // Loading calibration data from EEPROM.
  loadDataFromEeprom(&samples);
  samples.timeWithoutWind = calcTimeWithoutWind(samples.temperature);
  putText("INITIALIZED");

  while(1) {
    static bool sendUltrasoundFlag = true;
    static bool collectSecondDatasetFlag = false;

    // Send ultrasonic impulse.
    if(sendUltrasoundFlag) {
      sendUltrasoundFlag = false;
      startGenerator();
    }

    // Received signal.
    if(dataCollected) {
      dataCollected = false;

      // Received only first signal.
      if(!collectSecondDatasetFlag) {
        saveFirstDataset(&samples);
        collectSecondDatasetFlag = true;
        sendUltrasoundFlag = true;
      } else {
        collectSecondDatasetFlag = false;
        saveSecondDatasetAndProcessData(&samples);

        // Signal will be saved as calibration data.
        if(calibrationInProgress) {
          saveCalibratedData(&samples);
          samples.timeWithoutWind = calcTimeWithoutWind(samples.temperature);
          sendUltrasoundFlag = true;
        } else {
          continueProcessData(&samples);
          sendUltrasoundFlag = true;
        }
      }
    }
  }
  return 0;
}

void timerGeneratorInit(void) {
  // Setting generator pin as output.
  GENERATOR_PORT &= ~_BV(GENERATOR_OUT);
  GENERATOR_DDR |= _BV(GENERATOR_OUT);
  // Setting multiplexer pin as output.
  GENERATOR_PORT &= ~_BV(MPLEXER_SWITCH);
  GENERATOR_DDR |= _BV(MPLEXER_SWITCH);

  // Toggles generator pin on timer compare match.
  TCCR1A |= _BV(COM1A0);
  // CTC mode.
  TCCR1B |= _BV(WGM12);
  OCR1A = GENERATOR_TIMER_TICKS;
  // Interrupt on compare match.
  TIMSK1 |= _BV(OCIE1A);
}

void timerLedInit(void) {
  // Setting LED pin as output.
  LED_PORT &= ~_BV(LED);
  LED_DDR |= _BV(LED);
}

void timerCalibrationInit(void) {
  // Sets prescaler to 1024.
  TCCR0B |= _BV(CS02) | _BV(CS00);
  // Interrupt on overflow.
  TIMSK0 |= _BV(TOIE0);
}

void init(void) {
  // Enabling Watchdog.
  wdt_enable(WDTO_8S);

  // Setting pullup resistors for all pins.
  PORTA = 0xFF;
  PORTB = 0xFF;
  PORTC = 0xFF;
  PORTD = 0xFF;

  lcdInit();
  putText("INITIALIZING");
  timerGeneratorInit();
  timerCalibrationInit();
  spiInit();
  timerLedInit();
  // Enabling global interrupts.
  sei();
}

void startGenerator(void) {
  TCNT1 = 0; 
  // Starting timer (without prescaler).
  TCCR1B |= _BV(CS10);
}

void saveFirstDataset(Samples *samples) {
  // Converting raw data received from ADC.
  for(uint16_t i = 0; i < SAMPLE_NUMBER; i++) {
    samplesTmp[i] >>= 1;
    samplesTmp[i] &= 0x0FFF;
    samples->left[i] = samplesTmp[i];
  }

  // Switching multiplexer.
  GENERATOR_PORT |= _BV(MPLEXER_SWITCH);
}

void saveSecondDatasetAndProcessData(Samples *samples) {
  // Switching multiplexer.
  GENERATOR_PORT &= ~_BV(MPLEXER_SWITCH);

  uint32_t meanLeft = 0;
  uint32_t meanRight = 0;

  // Converting raw data received from ADC.
  for(uint16_t i = 0; i < SAMPLE_NUMBER; i++) {
    samplesTmp[i] >>= 1;
    samplesTmp[i] &= 0x0FFF;
    samples->right[i] = samplesTmp[i];
    
    meanLeft += samples->left[i];
    meanRight += samples->right[i];
  }

  meanLeft /= SAMPLE_NUMBER;
  meanRight /= SAMPLE_NUMBER;

  // Subtracting mean value of signal.
  for(uint16_t i = 0; i < SAMPLE_NUMBER; i++) {
    samples->left[i] -= meanLeft;
    samples->right[i] -= meanRight;
  }

  // Calculating envelopes of signals.
  envelope(samples);
}

void continueProcessData(Samples *samples) {
  Processed processed = {
    .deltaTimeLeft = 0,
    .deltaTimeRight = 0,
    .highestCorrelationLeft = {0},
    .highestCorrelationRight = {0},
    .highestCorrelationLength = REGRESSION_SAMPLES
  };

  // Calculating cross correlation of signals.
  crossCorrelation(samples,  &processed, MAX_DELAY);

  // Calculating delay (in ns) between cross correlated signals.
  processed.deltaTimeLeft *= 6333;
  processed.deltaTimeRight *= 6333;

  // Interpolating for better delay.
  processed.deltaTimeLeft += quadRegression(processed.highestCorrelationLeft,
                                            REGRESSION_SAMPLES);
  processed.deltaTimeRight += quadRegression(processed.highestCorrelationRight,
                                             REGRESSION_SAMPLES);

  // Calculating wind value.
  int32_t timeWithWindLeft;
  timeWithWindLeft = processed.deltaTimeLeft + samples->timeWithoutWind;
  int32_t timeWithWindRight;
  timeWithWindRight = processed.deltaTimeRight + samples->timeWithoutWind;
  int64_t windSpeed = 100000000LL * TRANSCEIVER_DISTANCE;
  windSpeed *= (timeWithWindRight - timeWithWindLeft);
  windSpeed /= 2LL * timeWithWindLeft * timeWithWindRight;

  bool directionRight;
  if(windSpeed >= 0) {
    directionRight = true;
  } else {
    directionRight = false;
    windSpeed *= -1;
  }

  // Displaying wind value.
  char stringToDisplay[17];
  sprintf(stringToDisplay,"%.2d,%.2d m/s   %s", (int16_t)(windSpeed / 100),
          (int16_t)(windSpeed % 100), directionRight ? "-->" : "<--");
  putText(stringToDisplay);
}

inline uint64_t calcTimeWithoutWind(int8_t temperature) {
  return (1000000000ULL * TRANSCEIVER_DISTANCE / (331300 + 606 * temperature));
}

/**
  * @brief  Interrupt occurs during sending impulses to ultrasonic transceiver.
  */
ISR(TIMER1_COMPA_vect) {
  static uint8_t cycles = 0;
  cycles++;

  // After 20 periods.
  if(cycles == 40) {
    // Timer stop.
    TCCR1B &= ~_BV(CS10);
    cycles = 0;

    // Wait for ultrasonic impulse to travel to another transceiver.
    _delay_us(50);

    // Collect signal data from transceiver.
    for(uint16_t i = 0; i < SAMPLE_NUMBER; i++) {
      // This operation takes exactly 152 cycles
      // which is 6333 ns for 24 MHz clock.
      getDataFromAdc(&samplesTmp[i]);
    }
    dataCollected = true;
  }
}

/**
  * @brief  Interrupt occurs regularly every 10.92 ms.
  *         Doesn't block other interrupt because its time is not critical.
  */
ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {
  static uint8_t tickCount = 0;
  static bool calibrationInitiated = false;

  static uint8_t tickLedCount = 0;
  tickLedCount++;

  // Switches status LED every 1 s.
  if(tickLedCount == 92) {
    tickLedCount = 0;
    LED_PORT ^= _BV(LED);
  }

  // If calibration button is hold for at least 2 s, calibration will occur.
  if(~(BUTTON_PIN) & _BV(BUTTON_CALIBRATION)) {
    tickCount++;
    if(!calibrationInitiated && tickCount == 183) {
      calibrationInitiated = true;
      calibrationInProgress = true;
    }
  } else {
    tickCount = 0;
    if(!calibrationInProgress)
    calibrationInitiated = false;
  }
}
