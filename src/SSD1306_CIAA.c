/*!
 *
 * @section intro_sec Introduction
 *
 * This is documentation for Adafruit's SSD1306 library for monochrome
 * OLED displays: http://www.adafruit.com/category/63_98
 *
 * These displays use I2C or SPI to communicate. I2C requires 2 pins
 * (SCL+SDA) and optionally a RESET pin. SPI requires 4 pins (MOSI, SCK,
 * select, data/command) and optionally a reset pin. Hardware SPI or
 * 'bitbang' software SPI are both supported.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a
 * href="https://github.com/adafruit/Adafruit-GFX-Library"> Adafruit_GFX</a>
 * being present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * @section license License
 *
 * BSD license, all text above, and the splash screen included below,
 * must be included in any redistribution.
 *
 */


/*
 * SSD1306_CIAA.c
 *
 * Libreria adaptada para manejar pantallas OLED con controlador SSD1306 en la EDU-CIAA
 * usando el puerto I2C. Con este proposito tambien se adapto parte de la libreria
 * Adafruit_GFX necesaria para algunas funciones.
 *
 * Modificada por Federico Meghinasso. (fmeghi@gmail.com)
 *
 */

/*================================Inclusiones====================================*/
#include "SSD1306_CIAA.h"
#include "splash.h"
/*================================Definiciones===================================*/



/*================================Variables Globales==============================*/

uint8_t *buffer;
ssd1306Config_t display;

/*================================Declarcion de funciones privadas===============*/

void ssd1306_commandList(uint8_t *data, uint8_t nData);
void ssd1306_command1(uint8_t data);

/*================================Funciones Publicas=============================*/

/*
 * Esta funcion se debe llamar antes de utilizar la pantalla.
 *
 *
 */

bool_t ssd1306_begin(uint8_t width, uint8_t height, i2cMap_t i2cNum,
		uint32_t i2cClock, uint8_t i2cAddress, uint8_t vccSource, gpioMap_t rstPin) {

	display.width = width;
	display.height = height;
	display.i2cNum = i2cNum;
	display.i2cClock = i2cClock;
	display.i2cAddress = i2cAddress;
	display.vccSource = vccSource;
	display.rstPin = rstPin;

	if ((!buffer) && !(buffer = (uint8_t *) malloc(display.width * ((display.height + 7) / 8))))
		return FALSE;

	ssd1306_clearDisplay();
	drawBitmap((display.width - splash1_width) / 2, (display.height - splash1_height) / 2,
			splash1_data, splash1_width, splash1_height, 1);


	gpioInit(display.rstPin, GPIO_OUTPUT);
	gpioWrite(display.rstPin, ON);
	delay(1);
	gpioWrite(display.rstPin, OFF);
	delay(10);
	gpioWrite(display.rstPin, ON);

	/*Se inicializa el bus I2C*/
	if (!i2cInit(display.i2cNum, display.i2cClock)) {
		return FALSE;
	}


	static uint8_t init1[] = {SSD1306_DISPLAYOFF,         // 0xAE
									SSD1306_SETDISPLAYCLOCKDIV,// 0xD5
									0x80,// the suggested ratio 0x80
									SSD1306_SETMULTIPLEX}; // 0xA8
	ssd1306_commandList(init1, sizeof(init1));
	ssd1306_command1(display.height - 1);

	static uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET, // 0xD3
									0x0,// no offset
									SSD1306_SETSTARTLINE | 0x0,// line #0
									SSD1306_CHARGEPUMP};        // 0x8D
	ssd1306_commandList(init2, sizeof(init2));

	ssd1306_command1((display.vccSource == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

	static uint8_t init3[] = {SSD1306_MEMORYMODE, // 0x20
									0x00,// 0x0 act like ks0108
									SSD1306_SEGREMAP | 0x1,
									SSD1306_COMSCANDEC};
	ssd1306_commandList(init3, sizeof(init3));



	ssd1306_command1(SSD1306_SETCOMPINS);
	ssd1306_command1(COMPINS);
	ssd1306_command1(SSD1306_SETCONTRAST);
	ssd1306_command1((display.vccSource == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);

	ssd1306_command1(SSD1306_SETPRECHARGE); // 0xd9
	ssd1306_command1((display.vccSource == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);

	static uint8_t init5[] = {SSD1306_SETVCOMDETECT, // 0xDB
									0x40,
									SSD1306_DISPLAYALLON_RESUME, // 0xA4
									SSD1306_NORMALDISPLAY, // 0xA6
									SSD1306_DEACTIVATE_SCROLL,
									SSD1306_DISPLAYON }; // Main screen turn on
	ssd1306_commandList(init5, sizeof(init5));

	return TRUE;
}


void ssd1306_display(void) {
	static uint8_t dlist1[] = { SSD1306_PAGEADDR, 0,       // Page start address
			0xFF,                   // Page end (not really, but works here)
			SSD1306_COLUMNADDR, 0 }; // Column start address
	ssd1306_commandList(dlist1, sizeof(dlist1));
	ssd1306_command1(display.width - 1); // Column end address

	uint16_t lengVec = (display.width * ((display.height + 7) / 8)) + 1;

	uint8_t outVec[lengVec];
	outVec[0] = 0x40; //Inicio de linea
	for (uint16_t i = 1; i < lengVec; i++) {
		outVec[i] = buffer[i - 1];
	}
	i2cWrite(display.i2cNum, display.i2cAddress, outVec, lengVec, TRUE);

}

void ssd1306_clearDisplay (void)
{
	memset(buffer, 0, display.width * ((display.height + 7) / 8));
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x >= 0) && (x < display.width) && (y >= 0) && (y < display.height)) {
		switch (color) {
		case SSD1306_WHITE:
			buffer[x + (y / 8) * display.width] |= (1 << (y & 7));
			break;
		case SSD1306_BLACK:
			buffer[x + (y / 8) * display.width] &= ~(1 << (y & 7));
			break;
		case SSD1306_INVERSE:
			buffer[x + (y / 8) * display.width] ^= (1 << (y & 7));
			break;
		}
	}
}


void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
		int16_t h, uint16_t color) {

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				byte <<= 1;
			else
				byte = bitmap[j * byteWidth + i / 8];
			if (byte & 0x80)
				drawPixel(x + i, y, color);
		}
	}
}

uint8_t getWidth(void){
	return display.width;
}

uint8_t getHeight(void){
	return display.height;
}

/*!
    @brief  Activate a right-handed scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
void startscrollright(uint8_t start, uint8_t stop) {

	static uint8_t scrollList1a[] = {
			SSD1306_RIGHT_HORIZONTAL_SCROLL, 0X00};
	ssd1306_commandList(scrollList1a, sizeof(scrollList1a));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static uint8_t scrollList1b[] = { 0X00, 0XFF,
			SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList1b, sizeof(scrollList1b));
}

/*!
    @brief  Activate a left-handed scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
void startscrollleft(uint8_t start, uint8_t stop) {
	static uint8_t scrollList2a[] =
			{ SSD1306_LEFT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList2a, sizeof(scrollList2a));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static uint8_t scrollList2b[] = { 0X00, 0XFF,
			SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList2b, sizeof(scrollList2b));

}

/*!
    @brief  Activate a diagonal scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/

void startscrolldiagright(uint8_t start, uint8_t stop) {

	static uint8_t scrollList3a[] = {
			SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00 };
	ssd1306_commandList(scrollList3a, sizeof(scrollList3a));
	ssd1306_command1(display.height);
	static uint8_t scrollList3b[] = {
			SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList3b, sizeof(scrollList3b));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static uint8_t scrollList3c[] = { 0X01, SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList3c, sizeof(scrollList3c));

}

/*!
    @brief  Activate alternate diagonal scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/

void startscrolldiagleft(uint8_t start, uint8_t stop) {

	static uint8_t scrollList4a[] = {
			SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00 };
	ssd1306_commandList(scrollList4a, sizeof(scrollList4a));
	ssd1306_command1(display.height);
	static uint8_t scrollList4b[] = {
			SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_commandList(scrollList4b, sizeof(scrollList4b));
	ssd1306_command1(start);
	ssd1306_command1(0X00);
	ssd1306_command1(stop);
	static uint8_t scrollList4c[] = { 0X01, SSD1306_ACTIVATE_SCROLL };
	ssd1306_commandList(scrollList4c, sizeof(scrollList4c));

}

/*!
    @brief  Cease a previously-begun scrolling action.
    @return None (void).
*/
void stopscroll(void) {
  ssd1306_command1(SSD1306_DEACTIVATE_SCROLL);

}



/*!
    @brief  Enable or disable display invert mode (white-on-black vs
            black-on-white).
    @param  i
            If true, switch to invert mode (black-on-white), else normal
            mode (white-on-black).
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed, rather a
            different pixel mode of the display hardware is used. When
            enabled, drawing SSD1306_BLACK (value 0) pixels will actually draw
   white, SSD1306_WHITE (value 1) will draw black.
*/
void invertDisplay(bool i) {
	ssd1306_command1(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}




/*==============================Funciones Privadas=================================*/

void ssd1306_commandList(uint8_t *data, uint8_t nData) {
	uint8_t outVec[nData + 1];
	outVec[0] = 0x00; //cabecera para envio de comandos
	for (uint8_t i = 1; i < (nData + 1); i++) {
		outVec[i] = data[i - 1];
	}
	i2cWrite(display.i2cNum, display.i2cAddress, outVec, nData + 1, TRUE);
}

void ssd1306_command1(uint8_t data) {
	uint8_t head[] = { 0x00, data };
	i2cWrite(display.i2cNum, display.i2cAddress, head, 2, TRUE);
}


