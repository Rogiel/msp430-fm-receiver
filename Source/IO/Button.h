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
		Button(Pin pin) : _pin(pin) {
			_pin.pullup(true);
		}

		Button(const Button& other) = default;
		Button& operator=(const Button& other) = default;

	public:
		inline bool isPressed() const {
			return _pin == false;
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
			bool state = Button::isPressed();
			if(!state) {
				return state;
			}

			for(int i = 0; i<1000; i++) {
				if(!Button::isPressed()) {
					return false;
				}
			}

			return true;
		}
	};
}


