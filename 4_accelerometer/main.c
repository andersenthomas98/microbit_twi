#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "twi.h"
#include "gpio.h"
#include "uart.h"
#include <stdio.h>
#define BTN_A 17
#define BTN_B 26


/* ************************

Vi sliter med oppgave 6.4.1 "Les WHO_AM_I".
Det virker som at vi ikke får lagt til data i data_buffer. Ser du noe vi ikke ser?

*************************** */

int main() {
	uart_init();
	twi_init();

	// Configure LED Matrix
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}
	
	uint8_t *data_buffer;
	data_buffer = (uint8_t *)malloc(8 * sizeof(uint8_t));
	memset(data_buffer, 0, 8);
	
	
	while(1) {
		twi_multi_read(0x1D, 0x0D, 1, data_buffer);
		if (data_buffer[0] == 0x5A) {
			// turn on

			for (int i=13; i<=15; i++) {
				GPIO->OUTSET = (1 << i);		
			}
		}
		free(data_buffer);
	}

	return 0;	
}
