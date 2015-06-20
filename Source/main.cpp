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

#include "FM/FMReceiver.h"
#include "FM/FMMemory.h"

/**
 * Main
 */
int main ()  {
	// Para o contador watchdog
	WDTCTL = WDTPW | WDTHOLD;

	FM::FMMemory& memory = FM::FMMemory::sharedMemory();
	memory._memory[0]._switches = 20;

	I2C::I2CBus bus(IO::Pin::get<1>(0), IO::Pin::get<1>(6));
	FM::FMReceiver receiver(bus);

	IO::Button button = IO::Pin::get<1>(3);

	receiver.setFrequency(96.0);
	receiver.setVolume(1.0);
	while(true) {
		receiver.updateIfNeeded();

		if(button.isPressed()) {
			receiver.setFrequency(98.0);
		}
	}
}