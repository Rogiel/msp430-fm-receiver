//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include <msp430.h>

#include "I2C/I2CBus.h"

/**
 * Main
 */
int main (void)  {
	// Para o contador watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// define direação da porta -> saída
	P1DIR = 0xFF;

	// liga o LED vermelho
	P1OUT = 0b0000000;

	I2C::I2CBus bus((I2C::I2CBus::Port)&P1OUT, (I2C::I2CBus::Port)&P1IN, (I2C::I2CBus::Port)&P2OUT, (I2C::I2CBus::Port)&P2IN);
	for(;;) {
		bus.start();
		bus.transmit(0x00);
		bus.stop();
	}

	return 1;
}