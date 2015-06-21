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

#include <msp430.h>

#include "Application.h"

/**
 * Main
 */
int main ()  {
	// Para o contador watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// configura o clock interno para 16 MHz
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL  = CALDCO_16MHZ;

	// configura o controlador de memória flash
	FCTL2 = FWKEY + FSSEL_2 + FN5 + FN3;

	Application& application = Application::sharedApplication();
	return application.run();
}
