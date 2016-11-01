/**
  ******************************************************************************
  * @file    lcd.c
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Display driver.
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

#include "lcd.h"

#include <string.h>

#include "hd44780/hd44780.h"

void lcdInit(void) {
  hd44780_init();
  hd44780_outcmd(HD44780_CLR);
  hd44780_wait_ready(0);
  // Auto-increment address counter.
  hd44780_outcmd(HD44780_ENTMODE(1, 0));
  hd44780_wait_ready(0);
  // Display ON. Cursor OFF.
  hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));
  hd44780_wait_ready(0);
}

void putText(const char *text) {
  hd44780_wait_ready(0);
  // Clearing display.
  hd44780_outcmd(HD44780_CLR);
  for(uint8_t i = 0; i < 16; i++) {
    hd44780_wait_ready(0);
    if(i < strlen(text))
      hd44780_outdata(text[i]);
    else
      break;
  }
}

void updateProgressBar(uint8_t n) {
  hd44780_wait_ready(0);
  // Setting address at second line.
  hd44780_outcmd(HD44780_DDADDR(0x40));
  for(uint8_t i = 0; i < 16; i++) {
    hd44780_wait_ready(0);
    if(i < n)
      hd44780_outdata(0xFF);
    else
      hd44780_outdata(' ');
  }
}
