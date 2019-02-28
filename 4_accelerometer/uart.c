#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "uart.h"
#include <sys/types.h>
#include <unistd.h>
#define TGT_RXD 25
#define TGT_TXD 24
#define BAUD9600 0x00275000
#define DISCONNECT 0xFFFFFFFF
#define ENABLE_UART 4


void uart_init() {
	GPIO->DIRSET = (0 << TGT_RXD);
	GPIO->DIRSET = (1 << TGT_TXD);
	UART->PSELTXD = TGT_TXD;
	UART->PSELRXD = TGT_RXD;
	UART->BAUDRATE = BAUD9600;
	UART->PSELCTS = DISCONNECT;
	UART->PSELRTS = DISCONNECT;
	UART->ENABLE = ENABLE_UART;
	UART->STARTRX = 1;
	// OBS: Burde muligens legges til en delay her
}

void uart_send(char letter) {
	UART->STARTTX = 1;
	UART->TXD = letter;
	UART->TXDRDY = 0;
	while (UART->TXDRDY == 0) {
		// run, run, run til TXDRDY blir 1 og vi vet at overføringen er ferdig.
	}
	UART->STOPTX = 1;
}

char uart_read() {
	UART->RXTO = 0;
	UART->STARTRX = 1;
	UART->RXDRDY = 0;
	while (UART->RXDRDY == 0) {
		// kjør fram til RXDRDY blir satt til 1	og vi vet at en ny byte har blitt lagt til i RXD
	}
	char letter = UART->RXD;
	
	return letter;
}


/*ssize_t _write(int fd, const void *buf, size_t count) {
	char* letter = (char *)(buf);
	for (int i=0; i<count; i++) {
		uart_send(*letter);
		letter++;			
	}
	return count;
}*/
