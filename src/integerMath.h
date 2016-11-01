/**
  ******************************************************************************
  * @file    integerMath.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of math functions operating on integers.
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

#ifndef INTEGERMATH_H_
#define INTEGERMATH_H_

#include <stdint.h>

/**
  * @brief  Integer square root function.
  * @param  op Argument of the function.
  * @retval Square root of the argument.
  */
uint16_t integerSqrt(uint32_t op);

/**
  * @brief  Integer division with rounding to nearest integer.
  * @param  a Numerator.
  * @param  b Denominator.
  * @retval Result of division.
  */
int32_t integerDivisonRound(int32_t a, int16_t b);

#endif /* INTEGERMATH_H_ */
