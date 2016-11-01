/**
  ******************************************************************************
  * @file    lcd.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    08-August-2015
  * @brief   Header file of display driver.
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

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

/**
  * @brief  LCD initialization.
  * @retval Void.
  */
void lcdInit(void);

/**
  * @brief  Displays text on LCD.
  * @param  text Text to display.
  * @retval Void.
  */
void putText(const char *text);

/**
  * @brief  Displays progress bar on LCD.
  * @param  n Number of rectangles to be displayed.
  * @retval Void.
  */
void updateProgressBar(uint8_t n);

#endif /* LCD_H_ */
