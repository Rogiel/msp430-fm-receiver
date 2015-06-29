//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//
// https://github.com/Rogiel/msp430-fm-receiver
//

#include "Application.h"

/**
 * Main
 */
int main () {
	WDTCTL = WDTPW | WDTHOLD;

	Application application;
	application.init();
	while(true) {
		application.loop();
	}
}
