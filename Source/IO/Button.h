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

#pragma once

#include "Pin.h"

#include <stdint.h>
#include <stdbool.h>

namespace IO {
	/**
	 * Esta classe implementa uma abstração sobre um pino que permite que a entrada seja interpretada como um botão.
	 */
	class Button {
	public:
		/**
		 * A estrutura de baixo nível que da acesso ao botão
		 */
		Pin _pin;

	public:
		/**
		 * Cria um novo botão no pino "pin"
		 *
		 * @param pin o pino para ler o sinal do botão
		 */
		inline Button(Pin pin) : _pin(pin) {
//			_pin.input();
			_pin.pullup(true);
		}

		/**
		 * Construtor de cópia
		 *
		 * @param other a instância para copiar
		 */
		Button(const Button& other) = default;

		/**
		 * Operador de cópia
		 *
		 * @param other a instância para copiar
		 * @return this
		 */
		Button& operator=(const Button& other) = default;

	public:
		/**
		 * Verifica se o botão está pressionado.
		 *
		 * @return true se o botão está pressionado.
		 */
		inline virtual bool isPressed() const {
			return _pin == false;
		}

	public:
		/**
		 * Retorna uma instância para o botão padrão sobre a placa de desenvolvimento do MSP430.
		 */
		static Button getDefaultButton() {
			return Button(Pin::get<1>(3));
		}
	};

	/**
	 * Extende a classe botão se modo a permitir que a entrada seja filtrada para eventos de bouncing
	 */
	class DebouncedButton : public Button {
	public:
		/**
		 * Cria um novo botão com debouncing
		 *
		 * @param pin o pino para ler o sinal do botão
		 */
		inline DebouncedButton(Pin pin) : Button(pin) { }

		/**
		 * Verifica se o botão está pressionado.
		 *
		 * @note esta implementação aguarda até que o sinal esteja estabilizado por um curto período de tempo. Caso o
		 * botão ainda não esteja estabilizado, a funcão retorna (false) imediatamente.
		 *
		 * @return true se o botão está pressionado e finalizou o debouncing
		 */
		inline virtual bool isPressed() const {
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

	class PressAndHoldButton : public DebouncedButton {
	private:
		mutable bool _hold;

	public:
		/**
		 * Cria um novo botão com debouncing
		 *
		 * @param pin o pino para ler o sinal do botão
		 */
		inline PressAndHoldButton(Pin pin) : DebouncedButton(pin) { }

	public:
		/**
		 * Verifica se o botão está pressionado.
		 *
		 * @note esta implementação aguarda até que o sinal esteja estabilizado por um curto período de tempo. Caso o
		 * botão ainda não esteja estabilizado, a funcão retorna (false) imediatamente.
		 *
		 * @return true se o botão está pressionado e finalizou o debouncing
		 */
		inline virtual bool isPressed() const {
			bool state = DebouncedButton::isPressed();
			if(state) {
				if(_hold == false) {
					_hold = true;
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}

		inline bool isHolding() const {
			return _hold;
		}
	};
}


