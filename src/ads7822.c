/**
  ******************************************************************************
  * @file    ads7822.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   ADS7822 Driver.
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

#include "ads7822.h"

void spiInit(void) {
  // Setting pins with MOSI, SS and SCK lines as outputs.
  SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SS) | _BV(SPI_SCK);
  // Setting pin with MISO line as input.
  SPI_DDR &= ~_BV(SPI_MISO);
  // Driving MOSI, SCK to low state and disabling pullup resistor on MISO.
  SPI_PORT &= ~(_BV(SPI_MISO) | _BV(SPI_MOSI) | _BV(SPI_SCK));
  SPI_SS_HIGH;

  // Setting ATmega as SPI master and enabling SPI interface.
  // Dividing clock driving SPI by 8.
  SPCR |= _BV(SPR0) | _BV(MSTR)| _BV(SPE);
  SPSR |= _BV(SPI2X);
}
