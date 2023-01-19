/* The MIT License
 *
 * Copyright (c) 2020 Piotr Duba
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include <stdbool.h>
#include <stdint.h>

/*
 *         PCF8574 <-> HD44780
 *
 * I2C I/O   P7 P6 P5 P4 P3 P2 P1 P0
 * LCD       D7 D6 D5 D4 BL EN RW RS
 *
 * */

#define HI2C_DEF hi2c1

#define RS_PIN 0x01
#define RW_PIN 0x02
#define EN_PIN 0x04
#define BL_PIN 0x08

#define INIT_8_BIT_MODE	0x30
#define INIT_4_BIT_MODE	0x02

#define CLEAR_LCD	0x01

#define UNDERLINE_OFF_BLINK_OFF		0x0C
#define UNDERLINE_OFF_BLINK_ON		0x0D
#define UNDERLINE_ON_BLINK_OFF		0x0E
#define UNDERLINE_ON_BLINK_ON		0x0F

#define FIRST_CHAR_LINE_1	0x80
#define FIRST_CHAR_LINE_2	0xC0

struct lcd_disp {
	uint8_t addr;
	char f_line[17];
	char s_line[17];
	bool bl;
};

void lcd_init(struct lcd_disp * lcd);
void lcd_write(uint8_t addr, uint8_t data, uint8_t xpin);
void lcd_display(struct lcd_disp * lcd);
void lcd_clear(struct lcd_disp * lcd);

#endif /* INC_LCD_I2C_H_ */
