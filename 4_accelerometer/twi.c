#include <stdio.h>
#include "twi.h"
#include "gpio.h"

#define FREQUENCY_100kbps 0x01980000
#define SCL 0
#define SDA 30

void twi_init() {
	GPIO->PIN_CNF[SCL] |= 0b11000001100; // Dir = input; Drive strength = S0D1;
	GPIO->PIN_CNF[SDA] |= 0b11000001100;
	
	TWIO->ENABLE = 0; // TWI må være disabled når PSELSCL/PSELSDA konfigureres
	TWIO->PSELSCL = SCL;
	TWIO->PSELSDA = SDA;
	TWIO->FREQUENCY = FREQUENCY_100kbps;
	TWIO->ENABLE = 5; // Enable TWI
	TWIO->TXDSENT = 0;
	TWIO->ERROR = 0;
	TWIO->RXDREADY = 0;
	
}

void twi_multi_read(uint8_t slave_address, uint8_t start_register, int registers_to_read, uint8_t * data_buffer) {
	const int num_reg_const = registers_to_read;
	TWIO->ADDRESS = slave_address;	
	TWIO->STARTTX = 1; // Starter skriveoperasjon
	TWIO->TXDSENT = 0; // TXDSENT settes ikke automatisk til 0 når vi dytter en ny byte inn i TXD
	TWIO->TXD = start_register; // Overfør start_register til TWI-bussen
	while (!TWIO->TXDSENT); // Vent på ACK fra slaven (TXDSENT-hendelse blir generert når ACK er mottatt)
	TWIO->STARTRX = 1; // Starter leseoperasjon
	TWIO->RXDREADY = 0;
	while ((registers_to_read - 1) > 0) {
		while(!TWIO->RXDREADY);
		TWIO->RXDREADY = 0; // RXDREADY må cleares før vi leser RXD
		registers_to_read--;
		data_buffer[num_reg_const - registers_to_read - 1] = TWIO->RXD;

	}
	while(!TWIO->RXDREADY);
	TWIO->STOP = 1; // Send NACK, stopper å lese fra TWI-bussen
	data_buffer[num_reg_const - 1] = TWIO->RXD; // Les siste byte
	TWIO->RXDREADY = 0;
}








void twi_multi_write(uint8_t slave_address, uint8_t start_register, int registers_to_write, uint8_t * data_buffer) {
	TWIO->ADDRESS = slave_address;
	TWIO->STARTTX = 1;
	TWIO->TXDSENT = 0;
	
	
}
