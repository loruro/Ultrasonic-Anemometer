/**
  ******************************************************************************
  * @file    ds1820.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of DS1820 Driver.
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

#ifndef DS1820_H_
#define DS1820_H_

#include <stdint.h>

#define DS1820_CONVERT_T    0x44
#define DS1820_READ_SCRATCH 0xbe
#define DS1820_SKIP_ROM     0xcc

/**
  * @brief  Gets temperature from sensor.
  * @retval Measured temperature.
  */
int8_t ds1820GetTemperature(void);

#endif /* DS1820_H_ */
