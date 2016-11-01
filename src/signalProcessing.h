/**
  ******************************************************************************
  * @file    signalProcessing.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of signal processing functions.
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

#ifndef SIGNALPROCESSING_H_
#define SIGNALPROCESSING_H_

#include "anemometer.h"

/**
  * @brief  Computes two cross correlations at a time
  *         using samples in parameters.
  *         Outputs highest sample plus specified number of samples around it.
  * @param  samples Pointer to a Samples structure.
  * @param  processed Pointer to a Processed structure.
  * @param  maxDelay Maximum delay of cross correlation.
  * @retval Void.
  */
void crossCorrelation(Samples *samples, Processed *processed, uint8_t maxDelay);

/**
  * @brief  Computes envelopes of two signals at a time.
  *         Envelope is calculated using Hilbert transform.
  * @param  samples Pointer to a Samples structure.
  * @retval Void.
  */
void envelope(Samples *samples);

/**
  * @brief  Computes quadratic regression.
  * @param  samples Signal.
  * @param  length Number of samples.
  * @retval Maximum of quadratic function.
  */
int32_t quadRegression(int32_t *samples, uint8_t length);

#endif /* SIGNALPROCESSING_H_ */
