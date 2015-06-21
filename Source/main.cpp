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
#include "IO/Button.h"
#include "IO/FlashVariable.h"

#include "FM/FMReceiver.h"
#include "FM/FMMemory.h"

DECLARE_FLASH_VARIABLE(FM::FMMemory, fmMemory);

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

	IO::DebouncedButton button(IO::Pin::get<1>(3));

	I2C::I2CBus bus(IO::Pin::get<1>(0), IO::Pin::get<1>(6));
	FM::FMReceiver receiver(bus);

	receiver.setFrequency(97.5);
	while(true) {
		receiver.updateIfNeeded();

		if(button.isPressed()) {
			receiver.setFrequency(97.6);
		}
	}
}