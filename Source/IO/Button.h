//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#pragma once

#include "Pin.h"

#include <stdint.h>
#include <stdbool.h>

namespace IO {
	class Button {
	public:
		Pin _pin;

	public:
		Button(Pin pin) : _pin(pin) {}

		Button(const Button& other) = default;
		Button& operator=(const Button& other) = default;

	public:
		inline bool isPressed() const {
			return _pin == true;
		}

	public:
		static Button getDefaultButton() {
			return Button(Pin::get<1>(3));
		}
	};

	class DebouncedButton : public Button {
	public:
		DebouncedButton(Pin pin) : Button(pin) {};

		bool isPressed() const {

			bool state = (_pin == true);
			for(int i = 0; i<100; i++) {
			}

			if(_pin == state) {
				return (_pin == true);
			}
			return state;
		}

	private:
		void debounce() {

		}
	};
}


