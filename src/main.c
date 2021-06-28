/*
 * main.c
 *
 *  Created on: 6 jun. 2021
 *      Author: Federico Meghinasso
 *
 */


/*===========================[Inclusiones]============================*/
#include "LogoFIUBA.c"
#include "sapi.h" //Inclusion de las SAPI

#include "SSD1306_CIAA.h"


/*===========================[Definiciones]==========================*/

/**
 * Los siguiente defines son los parametros de la pantalla
 */
#define WIDTH  128
#define HEIGHT 64

#define OLED_RESET 		GPIO8 // Se eligio un pin de la ciaa
#define SCREEN_ADDRESS  0x3D
#define I2C_NUMBER		I2C0 //puerto i2c
#define I2C_CLOCK       400000 // Frecuencia de clock del bus i2c = 400kHz
#define VCS             SSD1306_SWITCHCAPVCC // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally

#define max(a,b) ((a)>(b)?(a):(b))

/*================[Declaracion de funciones privadas]===============*/
void testdrawline(void);
void testdrawrect(void);
void testfillrect(void);
void testdrawcircle(void);
void testfillcircle(void);
void testdrawroundrect(void);
void testfillroundrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);
void testdrawchar(void);
void testscrolltext(void);



/*==========================Funciones=================================*/

int main ( void )
{
	boardConfig(); // Inicializar y configurar la plataforma

	ssd1306_begin(WIDTH, HEIGHT, I2C_NUMBER, I2C_CLOCK, SCREEN_ADDRESS, VCS, OLED_RESET);
	ssd1306_display();
	delay(2000);
	ssd1306_clearDisplay();
	testdrawline();
	testdrawrect();
	testfillrect();
	testdrawcircle();
	testfillcircle();
	testdrawroundrect();
	testfillroundrect();
	testdrawtriangle();
	testfilltriangle();
	testdrawchar();
	testscrolltext();
	ssd1306_clearDisplay();
	drawBitmap(48, 0, LogoFIUBA, 32, 62, SSD1306_WHITE);
	ssd1306_display();
	delay(2000);
	invertDisplay(TRUE);
	delay(1000);
	invertDisplay(FALSE);
	delay(1000);

	while(1){

	}


}

/*=========================[funciones privadas]=========================*/


void testdrawline() {
  int16_t i;

  ssd1306_clearDisplay(); // Clear display buffer

  for(i=0; i<WIDTH; i+=4) {
    drawLine(0, 0, i, HEIGHT-1, SSD1306_WHITE);
    ssd1306_display(); // Update screen WIDTH each newly-drawn line
    delay(1);
  }
  for(i=0; i<HEIGHT; i+=4) {
    drawLine(0, 0, WIDTH-1, i, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }
  delay(250);

  ssd1306_clearDisplay();

  for(i=0; i<WIDTH; i+=4) {
    drawLine(0, HEIGHT-1, i, 0, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }
  for(i=HEIGHT-1; i>=0; i-=4) {
    drawLine(0, HEIGHT-1, WIDTH-1, i, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }
  delay(250);

  ssd1306_clearDisplay();

  for(i=WIDTH-1; i>=0; i-=4) {
    drawLine(WIDTH-1, HEIGHT-1, i, 0, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }
  for(i=HEIGHT-1; i>=0; i-=4) {
    drawLine(WIDTH-1, HEIGHT-1, 0, i, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }
  delay(250);

  ssd1306_clearDisplay();

  for(i=0; i<HEIGHT; i+=4) {
    drawLine(WIDTH-1, 0, 0, i, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }
  for(i=0; i<WIDTH; i+=4) {
    drawLine(WIDTH-1, 0, i, HEIGHT-1, SSD1306_WHITE);
    ssd1306_display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
	ssd1306_clearDisplay();

	for (int16_t i = 0; i < HEIGHT / 2; i += 2) {
		drawRect(i, i, WIDTH - 2 * i, HEIGHT - 2 * i, SSD1306_WHITE);
		ssd1306_display(); // Update screen with each newly-drawn rectangle
		delay(1);
	}

	delay(2000);
}

void testfillrect(void) {
	ssd1306_clearDisplay();

	for (int16_t i = 0; i < HEIGHT / 2; i += 3) {
		// The INVERSE color is used so rectangles alternate white/black
		fillRect(i, i, WIDTH - i * 2, HEIGHT - i * 2, SSD1306_INVERSE);
		ssd1306_display(); // Update screen with each newly-drawn rectangle
		delay(1);
	}

	delay(2000);
}

void testdrawcircle(void) {
	ssd1306_clearDisplay();

	for (int16_t i = 0; i < max(WIDTH, HEIGHT) / 2; i += 2) {
		drawCircle(WIDTH / 2, HEIGHT / 2, i, SSD1306_WHITE);
		ssd1306_display();
		delay(1);
	}

	delay(2000);
}


void testfillcircle(void) {
	ssd1306_clearDisplay();

	for (int16_t i = max(WIDTH,HEIGHT) / 2; i > 0; i -= 3) {
		// The INVERSE color is used so circles alternate white/black
		fillCircle(WIDTH / 2, HEIGHT / 2, i,
				SSD1306_INVERSE);
		ssd1306_display(); // Update screen with each newly-drawn circle
		delay(1);
	}

	delay(2000);
}

void testdrawroundrect(void) {
	ssd1306_clearDisplay();

	for (int16_t i = 0; i < HEIGHT / 2 - 2; i += 2) {
		drawRoundRect(i, i, WIDTH - 2 * i,
				HEIGHT - 2 * i, WIDTH / 4, SSD1306_WHITE);
		ssd1306_display();
		delay(1);
	}

	delay(2000);
}

void testfillroundrect(void) {
	ssd1306_clearDisplay();

	for (int16_t i = 0; i < HEIGHT / 2 - 2; i += 2) {
		// The INVERSE color is used so round-rects alternate white/black
		fillRoundRect(i, i, WIDTH - 2 * i, HEIGHT - 2 * i,
		HEIGHT / 4, SSD1306_INVERSE);
		ssd1306_display();
		delay(1);
	}

	delay(2000);
}

void testdrawtriangle(void) {
	ssd1306_clearDisplay();

	for (int16_t i = 0; i < max(WIDTH,HEIGHT) / 2; i += 5) {
		drawTriangle(
		WIDTH / 2, HEIGHT / 2 - i,
		WIDTH / 2 - i, HEIGHT / 2 + i,
		WIDTH / 2 + i, HEIGHT / 2 + i, SSD1306_WHITE);
		ssd1306_display();
		delay(1);
	}

	delay(2000);
}

void testfilltriangle(void) {
	ssd1306_clearDisplay();

	for (int16_t i = max(WIDTH,HEIGHT) / 2; i > 0; i -= 5) {
		// The INVERSE color is used so triangles alternate white/black
		fillTriangle(
		WIDTH / 2, HEIGHT / 2 - i,
		WIDTH / 2 - i, HEIGHT / 2 + i,
		WIDTH / 2 + i, HEIGHT / 2 + i, SSD1306_INVERSE);
		ssd1306_display();
		delay(1);
	}

	delay(2000);
}

void testdrawchar(void) {
	ssd1306_clearDisplay();

	setTextSize(1);      // Normal 1:1 pixel scale
	setTextColor(SSD1306_WHITE); // Draw white text
	setCursor(0, 0);     // Start at top-left corner

	// Not all the characters will fit on the display. This is normal.
	// Library will draw what it can and the rest will be clipped.
	for (int16_t i = 0; i < 256; i++) {
		if (i == '\n')
			write(' ');
		else
			write(i);
	}

	ssd1306_display();
	delay(2000);
}

void testscrolltext(void) {
	ssd1306_clearDisplay();

	setTextSize(2); // Draw 2X-scale text
	setTextColor(SSD1306_WHITE);
	setCursor(10, 0);
	print("scroll");
	ssd1306_display();      // Show initial text
	delay(100);

	// Scroll in various directions, pausing in-between:
	startscrollright(0x00, 0x0F);
	delay(2000);
	stopscroll();
	delay(1000);
	startscrollleft(0x00, 0x0F);
	delay(2000);
	stopscroll();
	delay(1000);
	startscrolldiagright(0x00, 0x07);
	delay(2000);
	startscrolldiagleft(0x00, 0x07);
	delay(2000);
	stopscroll();
	delay(1000);
}





