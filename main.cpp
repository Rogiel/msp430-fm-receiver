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

/**
 * Main
 */
int main (void)  {
	// Para o contador watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// define direação da porta -> saída
	P1DIR = 0xFF;

	// liga o LED vermelho
	P1OUT = 0b0000001;

	return 1;
}