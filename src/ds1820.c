/**
  ******************************************************************************
  * @file    ds1820.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   DS1820 Driver.
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

#include "ds1820.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include "anemometer.h"

/**
  * @brief  Pulls DQ line to low.
  * @retval Void.
  */
inline void ds1820PullLineLow(void);

/**
  * @brief  Releases DQ line.
  * @retval Void.
  */
inline void ds1820ReleaseLine(void);

/**
  * @brief  Resets sensor.
  * @retval Void.
  */
void ds1820Reset(void);

/**
  * @brief  Sends one bit to sensor.
  * @param  bit Bit to be sent to sensor.
  * @retval Void.
  */
void ds1820WriteBit(uint8_t bit);

/**
  * @brief  Sends command to sensor.
  * @param  byte Command byte to be sent to sensor.
  * @retval Void.
  */
void ds1820WriteCommand(uint8_t byte);

/**
  * @brief  Reads one bit from sensor.
  * @retval Read bit.
  */
uint8_t ds1820ReadBit(void);

/**
  * @brief  Reads temperature value from sensor.
  * @retval Measured temperature.
  */
int8_t ds1820ReadTemperature(void);

inline void ds1820PullLineLow(void) {
  DS1820_PORT &= ~_BV(DS1820_DQ);
  DS1820_DDR |= _BV(DS1820_DQ);
}

inline void ds1820ReleaseLine(void) {
  DS1820_DDR &= ~_BV(DS1820_DQ);
}

void ds1820Reset(void) {
  ds1820PullLineLow();
  _delay_us(480);

  ds1820ReleaseLine();
  _delay_us(480);
}

void ds1820WriteBit(uint8_t bit) {
  ds1820PullLineLow();
  _delay_us(1);

  if (bit) {
    ds1820ReleaseLine();
    _delay_us(60);
  } else {
    _delay_us(60);
    ds1820ReleaseLine();
  }    
}

void ds1820WriteCommand(uint8_t byte) {
  for(uint8_t i = 0; i < 8; i++) {
    ds1820WriteBit(byte & 0x01);
    byte >>= 1;
  }
}

uint8_t ds1820ReadBit(void) {
  ds1820PullLineLow();
  _delay_us(1);

  ds1820ReleaseLine();
  _delay_us(14);

  // Read value of bit.
  uint8_t bit = 0;
  if (DS1820_PIN & _BV(DS1820_DQ))
    bit = 1;

  _delay_us(45);
  return bit;
}

int8_t ds1820ReadTemperature(void) {
  // Ignore first bit.
  ds1820ReadBit();

  int8_t byte = 0;
  for(uint8_t i = 0; i < 8; i++) {
    byte |= (ds1820ReadBit() << i);
  }

  return byte;
}

int8_t ds1820GetTemperature(void) {
  // Disable global interrupts.
  cli();
  ds1820Reset();
  ds1820WriteCommand(DS1820_SKIP_ROM);
  ds1820WriteCommand(DS1820_CONVERT_T);

  // Wait for conversion to complete.
  while(!ds1820ReadBit());

  ds1820Reset();
  ds1820WriteCommand(DS1820_SKIP_ROM);
  ds1820WriteCommand(DS1820_READ_SCRATCH);

  int8_t temperature = ds1820ReadTemperature();
  // Enable global interrupts.
  sei();

  return temperature;
}
