/**
  ******************************************************************************
  * @file    calibrationEEPROM.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Functions for accessing calibration data in EEPROM.
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

#include "calibrationEEPROM.h"

#include <avr/eeprom.h>
#include <string.h>

#include "lcd.h"
#include "ds1820.h"

// Data stored in EEPROM.
EEMEM int16_t samplesInEepromLeft[SAMPLE_NUMBER];
EEMEM int16_t samplesInEepromRight[SAMPLE_NUMBER];
EEMEM uint8_t temperatureEEPROM;

bool calibrationInProgress = false;

void loadDataFromEeprom(Samples *samples) {
  eeprom_read_block(samples->fromEepromLeft, samplesInEepromLeft,
                    sizeof(samplesInEepromLeft));
  eeprom_read_block(samples->fromEepromRight, samplesInEepromRight,
                    sizeof(samplesInEepromRight));
  samples->temperature = (int8_t)eeprom_read_byte(&temperatureEEPROM);
}

void saveCalibratedData(Samples *samples) {
  putText("CALIBRATING");

  memcpy(samples->fromEepromLeft, samples->left,
         sizeof(samplesInEepromLeft));
  memcpy(samples->fromEepromRight, samples->right,
         sizeof(samplesInEepromRight));
  eeprom_update_block(samples->fromEepromLeft, samplesInEepromLeft,
                      sizeof(samplesInEepromLeft));
  eeprom_update_block(samples->fromEepromRight, samplesInEepromRight,
                      sizeof(samplesInEepromRight));

  samples->temperature = ds1820GetTemperature();
  eeprom_update_byte(&temperatureEEPROM, (uint8_t)samples->temperature);

  calibrationInProgress = false;
  putText("CALIBRATION DONE");
}
