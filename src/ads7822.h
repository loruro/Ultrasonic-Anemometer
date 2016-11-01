/**
  ******************************************************************************
  * @file    ads7822.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of ADS7822 Driver.
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

#ifndef ADS7822_H_
#define ADS7822_H_

#include "anemometer.h"

// Macro driving SS line to high state.
#define SPI_SS_HIGH SPI_PORT |= _BV(SPI_SS)

// Macro driving SS line to low state.
#define SPI_SS_LOW SPI_PORT &= ~_BV(SPI_SS)

/**
  * @brief  SPI interface initialization.
  * @retval Void.
  */
void spiInit(void);

/**
  * @brief  Gets data from ADC.
  * @param  data Data which will be read from ADC.
  * @retval Void.
  */
inline void getDataFromAdc(uint16_t *data) {
  // Activating ADC.
  SPI_SS_LOW;
  // Starting transmission.
  SPDR = 0x00;

  // Waiting for end of transmission.
  // NOP causes loop to check end of transmission flag at the most optimal time.
  asm volatile("nop"::);
  while(!(SPSR & _BV(SPIF)));

  // Starting another transmission.
  SPDR = 0x00;
  // Saving byte from previous transmission.
  *((uint8_t*)data + 1) = SPDR;

  // Waiting for end of transmission. This time two NOP's were needed.
  asm volatile(
    "nop\n\t"
    "nop\n\t"::
  );
  while(!(SPSR & _BV(SPIF)));

  // Saving byte from transmission.
  *((uint8_t*)data + 0) = SPDR;
  // Deactivating ADC.
  SPI_SS_HIGH;
}

#endif /* ADS7822_H_ */
