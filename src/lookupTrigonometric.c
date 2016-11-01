/**
  ******************************************************************************
  * @file    lookupTrigonometric.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Trigonometric functions using lookup tables.
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

#include "lookupTrigonometric.h"

#include "lookupTables.h"

uint8_t lookupSinSq(int32_t index) {
  if(index < 0)
    index *= -1;

  index %= PI_1000;

  return pgm_read_byte(&sinSqTable[index]);
}

int32_t lookupCot(int32_t index) {
  index %= PI_100000;

  if(index < 0)
    index += PI_100000 + 1;

  const uint16_t tableLength =  sizeof(cotTable_1) / sizeof(cotTable_1[0]);

  if (index <= PI_100000/2) {
    if(index < tableLength) {
      return pgm_read_dword(&cotTable_1[index]);
    } else {
      index -= tableLength;
      index /= 10;
      return pgm_read_word(&cotTable_2[index]);
    }
  } else {
    index = PI_100000 - index;

    if(index < tableLength) {
      return -pgm_read_dword(&cotTable_1[index]);
    } else {
      index -= tableLength;
      index /= 10;
      return -(int32_t)pgm_read_word(&cotTable_2[index]);
    }
  }
}
