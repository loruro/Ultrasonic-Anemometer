/**
  ******************************************************************************
  * @file    anemometer.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Configuration defines and struct definitions.
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

#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_

#include <avr/io.h>

// Number of samples in signal.
#define SAMPLE_NUMBER 480

// Maximum delay of cross correlation.
#define MAX_DELAY 36

// Number of regression samples.
#define REGRESSION_SAMPLES 31

// Distance between ultrasonic transceivers.
#define TRANSCEIVER_DISTANCE 270

// Ultrasound generator and multiplexer.
#define GENERATOR_PORT PORTD
#define GENERATOR_DDR  DDRD
#define GENERATOR_OUT  PD5
#define MPLEXER_SWITCH PD6
// This value causes generator to generate signal at frequency 40268 Hz.
#define GENERATOR_TIMER_TICKS 297

// Calibration button.
#define BUTTON_PIN         PINB
#define BUTTON_CALIBRATION PB0

// Status LED.
#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED      PB1

// SPI interface.
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK  PB7
#define SPI_SS   PB4

// DS1820 sensor.
#define DS1820_PORT PORTB
#define DS1820_DDR  DDRB
#define DS1820_PIN  PINB
#define DS1820_DQ   PB2

/**
  * @brief  Collected samples and other data needed for processing.
  */
typedef struct Samples {
  int16_t left[SAMPLE_NUMBER];            /**< Left transceiver samples from ADC. */
  int16_t right[SAMPLE_NUMBER];           /**< Right transceiver samples from ADC. */
  int16_t fromEepromLeft[SAMPLE_NUMBER];  /**< Left transceiver samples from EEPROM. */
  int16_t fromEepromRight[SAMPLE_NUMBER]; /**< Right transceiver samples from EEPROM. */
  int8_t temperature;                     /**< Measured temperature. */
  uint64_t timeWithoutWind;               /**< Calculated signal time of flight without wind. */
  uint16_t samplesNumber;                 /**< Number of samples for processing. */
} Samples;

/**
  * @brief  Data generated during signal processing.
  */
typedef struct Processed {
  int32_t deltaTimeLeft;                               /**< Left transceiver delay between cross correlated signals. */
  int32_t deltaTimeRight;                              /**< Right transceiver delay between cross correlated signals. */
  int32_t highestCorrelationLeft[REGRESSION_SAMPLES];  /**< Left transceiver highest samples from correlation. */
  int32_t highestCorrelationRight[REGRESSION_SAMPLES]; /**< Right transceiver highest samples from correlation. */
  uint8_t highestCorrelationLength;                    /**< Number of highest samples from correlation. */
} Processed;

#endif /* ANEMOMETER_H_ */
