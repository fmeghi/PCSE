/*
 * SSD1306_CIAA.c
 *
 *  Created on: 12 jun. 2021
 *      Author: Federico Meghinasso
 *      Mail: 	fmeghi@gmail.com
 */

/*================================Inclusiones====================================*/
#include "SSD1306_CIAA.h"
#include "splash.h"
/*================================Definiciones===================================*/

//#define ssd1306_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

/*================================Variables Globales==============================*/
uint8_t buffer[WIDTH * ((HEIGHT + 7) / 8)];



/*================================Declarcion de funciones privadas===============*/

void ssd1306_commandList(uint8_t *c, uint8_t n);
void ssd1306_command1(uint8_t data);

/*================================Funciones Publicas=============================*/

bool_t ssd1306_begin(void)
{

	ssd1306_clearDisplay();
	drawBitmap((WIDTH - splash1_width) / 2, (HEIGHT - splash1_height) / 2,
			splash1_data, splash1_width, splash1_height, 1);

	uint8_t vccstate = VCS;
	gpioInit(OLED_RESET, GPIO_OUTPUT);
	gpioWrite(OLED_RESET, ON);
	delay(1);
	gpioWrite(OLED_RESET, OFF);
	delay(10);
	gpioWrite(OLED_RESET, ON);

	/*Se inicializa el bus I2C*/
	if(!i2cInit(I2C_NUMBER, I2C_CLOCK))
	{
		return FALSE;
	}


	static uint8_t init1[] = {SSD1306_DISPLAYOFF,         // 0xAE
									SSD1306_SETDISPLAYCLOCKDIV,// 0xD5
									0x80,// the suggested ratio 0x80
									SSD1306_SETMULTIPLEX}; // 0xA8
	ssd1306_commandList(init1, sizeof(init1));
	ssd1306_command1(HEIGHT - 1);

	static uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET, // 0xD3
									0x0,// no offset
									SSD1306_SETSTARTLINE | 0x0,// line #0
									SSD1306_CHARGEPUMP};        // 0x8D
	ssd1306_commandList(init2, sizeof(init2));

	ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

	static uint8_t init3[] = {SSD1306_MEMORYMODE, // 0x20
									0x00,// 0x0 act like ks0108
									SSD1306_SEGREMAP | 0x1,
									SSD1306_COMSCANDEC};
	ssd1306_commandList(init3, sizeof(init3));



	ssd1306_command1(SSD1306_SETCOMPINS);
	ssd1306_command1(COMPINS);
	ssd1306_command1(SSD1306_SETCONTRAST);
	ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);

	ssd1306_command1(SSD1306_SETPRECHARGE); // 0xd9
	ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);

	static uint8_t init5[] = {SSD1306_SETVCOMDETECT, // 0xDB
									0x40,
									SSD1306_DISPLAYALLON_RESUME, // 0xA4
									SSD1306_NORMALDISPLAY, // 0xA6
									SSD1306_DEACTIVATE_SCROLL,
									SSD1306_DISPLAYON }; // Main screen turn on
	ssd1306_commandList(init5, sizeof(init5));

	return TRUE;
}


void ssd1306_display (void)
{
	static uint8_t dlist1[] = { SSD1306_PAGEADDR, 0,       // Page start address
									0xFF,                   // Page end (not really, but works here)
									SSD1306_COLUMNADDR, 0 }; // Column start address
	ssd1306_commandList(dlist1, sizeof(dlist1));
	ssd1306_command1(WIDTH - 1); // Column end address

	static uint16_t lengVec = (WIDTH * ((HEIGHT + 7) / 8)) + 1;

	uint8_t outVec[lengVec];
	outVec[0] = 0x40; //Inicio de linea
	for(uint16_t i = 1; i<lengVec; i++){
		outVec[i] = buffer[i - 1];
	}
	i2cWrite( I2C_NUMBER, SCREEN_ADDRESS, outVec, lengVec, TRUE );

}

void ssd1306_clearDisplay (void)
{
	memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
		switch (color) {
		case SSD1306_WHITE:
			buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
			break;
		case SSD1306_BLACK:
			buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
			break;
		case SSD1306_INVERSE:
			buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
			break;
		}
	}
}


void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                              int16_t w, int16_t h, uint16_t color) {

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

/*==============================Funciones Privadas=================================*/

void ssd1306_commandList(uint8_t *data, uint8_t n)
{
	uint8_t outVec[n + 1];
	outVec[0] = 0x00; //cabecera para envio de comandos
	for(uint8_t i = 1; i < (n + 1); i++){
		outVec[i] = data[i - 1];
	}
	i2cWrite( I2C_NUMBER, SCREEN_ADDRESS, outVec, n+1, TRUE );
}

void ssd1306_command1(uint8_t data)
{
	uint8_t head []= {0x00, data};
	i2cWrite( I2C_NUMBER, SCREEN_ADDRESS, head, 2, TRUE );
}



