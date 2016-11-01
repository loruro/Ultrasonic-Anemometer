/**
  ******************************************************************************
  * @file    signalProcessing.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Signal processing functions.
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

#include "signalProcessing.h"

#include <avr/wdt.h>
#include <string.h>

#include "integerMath.h"
#include "lookupTrigonometric.h"
#include "lcd.h"

void crossCorrelation(Samples *samples, Processed *processed,
                      uint8_t maxDelay) {
  uint16_t correlationLength = 2 * maxDelay + 1;

  int32_t correlationLeft[correlationLength];
  int32_t correlationRight[correlationLength];
  memset(correlationLeft, 0, correlationLength * sizeof(correlationLeft[0]));
  memset(correlationRight, 0, correlationLength * sizeof(correlationRight[0]));

  // Cross correlation computation.
  for (int16_t delay = -maxDelay; delay <= maxDelay; delay++) {
    for (uint16_t i = 0; i < samples->samplesNumber; i++) {
      int16_t j = i + delay;
      if (j < 0 || j >= samples->samplesNumber) {
        continue;
      } else {
        int32_t correlSample;
        correlSample = (int32_t)samples->left[i] * samples->fromEepromLeft[j];
        correlationLeft[delay + maxDelay] += correlSample;
        correlSample = (int32_t)samples->right[i] * samples->fromEepromRight[j];
        correlationRight[delay + maxDelay] += correlSample;
      }
    }
  }

  // Finding highest sample and samples around it.
  uint8_t offset = processed->highestCorrelationLength / 2;
  for (int16_t delay = offset; delay < correlationLength - offset; delay++) {
    if(correlationLeft[delay] > processed->highestCorrelationLeft[offset]) {
      uint8_t corrIndex = delay - offset;
      for(uint8_t i = 0; i < processed->highestCorrelationLength; i++)
        processed->highestCorrelationLeft[i] = correlationLeft[corrIndex + i];

      processed->deltaTimeLeft = delay - maxDelay;
    }

    if(correlationRight[delay] > processed->highestCorrelationRight[offset]) {
      uint8_t corrIndex = delay - offset;
      for(uint8_t i = 0; i < processed->highestCorrelationLength; i++)
        processed->highestCorrelationRight[i] = correlationRight[corrIndex + i];

      processed->deltaTimeRight = delay - maxDelay;
    }
  }
}

void envelope(Samples *samples) {
  uint16_t samplesNumber = samples->samplesNumber;
  int16_t envelopeLeft[samplesNumber];
  int16_t envelopeRight[samplesNumber];
  memset(envelopeLeft, 0, samplesNumber * sizeof(envelopeLeft[0]));
  memset(envelopeRight, 0, samplesNumber * sizeof(envelopeRight[0]));

  for(int16_t i = 0; i < samplesNumber; i++) {
    // Resetting Watchdog.
    // This loop is the longest part of the program.
    wdt_reset();

    // Discrete Hilbert transform.
    for(int16_t n = 0; n < samplesNumber; n++) {
      int16_t kn = i - n;
      uint8_t sin = lookupSinSq((int32_t)kn * PI_1000 / 2);
      uint32_t angle = integerDivisonRound(PI_100000 * kn, samplesNumber);
      int32_t cot = lookupCot(angle);
      
      int32_t h = sin * cot * 2 / 500 / 255;
      envelopeLeft[i] += h * samples->left[n] / samplesNumber;
      envelopeRight[i] += h * samples->right[n] / samplesNumber;
    }

    // Calculating envelope.
    uint32_t envelopeSquared = (int32_t)envelopeLeft[i] * envelopeLeft[i];
    uint32_t samplesSquared = (int32_t)samples->left[i] * samples->left[i];
    envelopeLeft[i] = integerSqrt(envelopeSquared + samplesSquared);
    
    envelopeSquared = (int32_t)envelopeRight[i] * envelopeRight[i];
    samplesSquared = (int32_t)samples->right[i] * samples->right[i];
    envelopeRight[i] = integerSqrt(envelopeSquared + samplesSquared);

    // Updating progress bar.
    if ((i + 1) % (samplesNumber / 16) == 0)
      updateProgressBar((i + 1) * 16 / samplesNumber);
  }

  memcpy(samples->left, envelopeLeft, sizeof(envelopeLeft));
  memcpy(samples->right, envelopeRight, sizeof(envelopeRight));
}

int32_t quadRegression(int32_t *samples, uint8_t length) {
  uint16_t xMean = 0;
  int64_t yMean = 0;
  uint16_t x2Mean = 0;
  uint32_t x3Mean = 0;
  uint32_t x4Mean = 0;
  int64_t xyMean = 0;
  int64_t x2yMean = 0;

  for(uint8_t i = 0; i < length; i++) {
    xMean += i + 1;
    yMean += samples[i];
    x2Mean += (uint16_t)(i + 1) * (i + 1);
    x3Mean +=  (uint32_t)(i + 1) * (i + 1) * (i + 1);
    x4Mean += (uint32_t)(i + 1) * (i + 1) * (i + 1) * (i + 1);
    xyMean += (uint64_t)(i + 1) * samples[i];
    x2yMean += (uint64_t)(i + 1) * (i + 1) * samples[i];
  }

  xMean /= length;
  yMean /= length;
  x2Mean /= length;
  x3Mean /= length;
  x4Mean /= length;
  xyMean /= length;
  x2yMean /= length;

  int16_t Sxx = x2Mean - xMean * xMean;
  int64_t Sxy = xyMean - xMean * yMean;
  int32_t Sxx2 = x3Mean - xMean * x2Mean;
  int32_t Sx2x2 = x4Mean - (uint32_t)x2Mean * x2Mean;
  int64_t Sx2y = x2yMean - x2Mean * yMean;

  int64_t tmp = (Sxx * Sx2x2 - Sxx2 * Sxx2);
  int64_t B = (Sxy * Sx2x2 - Sx2y * Sxx2) / tmp;
  int64_t A = (Sx2y * Sxx - Sxy * Sxx2) / tmp;

  int32_t result = -B * 6333 / (2 * A);
  int32_t middle = (length + 1) / 2;
  middle *= 6333;

  return result - middle;
}
