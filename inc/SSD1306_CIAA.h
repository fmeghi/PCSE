/*
 * SSD1306_CIAA.h
 *
 *  Created on: 12 jun. 2021
 *      Author: f_meg
 */

#ifndef SSD1306_CIAA_H_
#define SSD1306_CIAA_H_

/*===================================Inclusiones=========================================*/

#include "sapi.h"
#include "stdlib.h"
#include "string.h"




/*===================================Definiciones========================================*/


#define COMPINS			0x12


#define BLACK 	SSD1306_BLACK     ///< Draw 'off' pixels
#define WHITE 	SSD1306_WHITE     ///< Draw 'on' pixels
#define INVERSE SSD1306_INVERSE ///< Invert pixels

/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 	0   ///< Draw 'off' pixels
#define SSD1306_WHITE 	1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 			0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 			0x21          ///< See datasheet
#define SSD1306_PAGEADDR 			0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 		0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 			0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 			0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 		0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 		0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 		0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 		0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 			0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 			0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 			0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC			0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 	0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 		0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 			0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 		0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 	0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 	0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 	0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 	0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 	0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 				0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 					0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 	0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 	0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 						0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 						0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 				0xA3             ///< Set scroll range

/*==============================Definicion de tipos de dato==============================*/

typedef struct{
	uint8_t width;
	uint8_t height;
	i2cMap_t i2cNum;
	uint32_t i2cClock;
	uint8_t i2cAddress;
	uint8_t vccSource;
	gpioMap_t rstPin;
}ssd1306Config_t;


/*==============================Declaracion de funciones=================================*/

bool_t ssd1306_begin(uint8_t width, uint8_t height, i2cMap_t i2cNum,
		uint32_t i2cClock, uint8_t i2cAddress, uint8_t vccSource, gpioMap_t rstPin);
void ssd1306_display (void);
void ssd1306_clearDisplay (void);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
uint8_t getWidth(void);
uint8_t getHeight(void);
void startscrollright(uint8_t start, uint8_t stop);
void startscrollleft(uint8_t start, uint8_t stop);
void startscrolldiagright(uint8_t start, uint8_t stop);
void startscrolldiagleft(uint8_t start, uint8_t stop);
void stopscroll(void);
void invertDisplay(bool i);

//SSD1306_gfx
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners,
		int16_t delta, uint16_t color);
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                 int16_t r, uint16_t color);
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint16_t color);
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
		uint16_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color);
void setTextSize(uint8_t s_x);
void setTextColor(uint16_t c);
void setCursor(int16_t x, int16_t y);
void drawChar(int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y);
size_t write(uint8_t c);
void print(char* str);

#endif /* POSGRADO_PCSE_INC_SSD1306_CIAA_H_ */
