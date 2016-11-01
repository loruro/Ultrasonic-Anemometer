/**
  ******************************************************************************
  * @file    lookupTrigonometric.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of trigonometric functions using lookup tables.
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

#ifndef LOOKUPTRIGONOMETRIC_H_
#define LOOKUPTRIGONOMETRIC_H_

#include <stdint.h>

#define PI_1000 3142
#define PI_100000 314159

/**
  * @brief  Squared sine function.
  *         This function uses precalculated values of squared sine
  *         in form of lookup table.
  * @param  index Angle of sine.
  * @retval Value of squared sine.
  */
uint8_t lookupSinSq(int32_t index);

/**
  * @brief  Cotangent function.
  *         This function uses precalculated values of cotangent
  *         in form of lookup table.
  * @param  index Angle of cotangent.
  * @retval Value of cotangent.
  */
int32_t lookupCot(int32_t index);

#endif /* LOOKUPTRIGONOMETRIC_H_ */
