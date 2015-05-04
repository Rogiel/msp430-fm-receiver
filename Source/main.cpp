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
	P1DIR = 0b11111101;

	// liga o LED vermelho
	P1OUT = 0b0000000;

//	for(;;) {
//		if(P1IN & (1 << 3)) {
//			P1OUT ^= (P1OUT & 1 ? 0 : 1);
//		}
//	}

	IO::Pin scl((IO::Pin::Port) &P1IN, (IO::Pin::Port) &P1OUT, 0, (IO::Pin::Port) &P1DIR);
	IO::Pin sda((IO::Pin::Port) &P1IN, (IO::Pin::Port) &P1OUT, 6, (IO::Pin::Port) &P1DIR);

	I2C::I2CBus bus(scl, sda);
	bus.start();
	bus.transmit(0b10101010);
	bus.stop();

	P1OUT = 0;
//	for(;;) {
//		bus.start();
//		bus.transmit(0x00);
//		bus.stop();
//	}

	return 1;
}