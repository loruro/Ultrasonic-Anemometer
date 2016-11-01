/**
  ******************************************************************************
  * @file    integerMath.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Math functions operating on integers.
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

#include "integerMath.h"

uint16_t integerSqrt(uint32_t op) {
  uint32_t res = 0;
  uint32_t one = (uint32_t)1 << 30;

  while (one > op)
    one >>= 2;

  while (one != 0) {
    if (op >= res + one) {
      op = op - (res + one);
      res = res +  2 * one;
    }
    res >>= 1;
    one >>= 2;
  }

  if (op > res)
    res++;

  return (uint16_t)res;
}

int32_t integerDivisonRound(int32_t a, int16_t b) {
  return (a < 0) ? ((a - b / 2) / b) : ((a + b / 2) / b);
}
