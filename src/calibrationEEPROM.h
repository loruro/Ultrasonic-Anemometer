/**
  ******************************************************************************
  * @file    calibrationEEPROM.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of functions for accessing calibration data in EEPROM.
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

#ifndef CALIBRATIONEEPROM_H_
#define CALIBRATIONEEPROM_H_

#include <stdbool.h>

#include "anemometer.h"

// Flag indicating if calibration is in progress.
extern bool calibrationInProgress;

/**
  * @brief  Loads calibration data from EEPROM.
  * @param  samples Pointer to a Samples structure.
  * @retval Void.
  */
void loadDataFromEeprom(Samples *samples);

/**
  * @brief  Saves calibration data to EEPROM.
  * @param  samples Pointer to a Samples structure.
  * @retval Void.
  */
void saveCalibratedData(Samples *samples);

#endif /* CALIBRATIONEEPROM_H_ */
