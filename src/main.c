/*
 * main.c
 *
 *  Created on: 6 jun. 2021
 *      Author: Federico Meghinasso
 *
 */


/*========================Inclusiones================================*/
#include "sapi.h" //Inclusion de las SAPI

#include "SSD1306_CIAA.h"



int main ( void )
{
	boardConfig(); // Inicializar y configurar la plataforma

	ssd1306_begin();
	ssd1306_display();
	ssd1306_clearDisplay();
	ssd1306_display();

	while(1){

	}


}
