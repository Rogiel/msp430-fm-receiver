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

#include <stdint.h>
#include <stdbool.h>

namespace IO {
	/**
	 * Representa um pino de entrada e saída
	 */
	class Pin {
	public:
		/**
		 * Um alias para o tipo Porta
		 */
		typedef uint8_t* Port;

		/**
		 * Um alias para o tipo Bit
		 */
		typedef uint8_t Bit;

	private:
		/**
		 * O endereço da porta de leitura
		 */
		Port _readPort;

		/**
		 * O endereço da porta de escrita
		 */
		Port _writePort;

		/**
		 * O endereço do registrador de direção da porta
		 */
		Port _direction;

		/**
		 * O bit que deve escrito/lido
		 */
		Bit _bit;

	public:
		/**
		 * Cria um novo pino.
		 *
		 * @param readPort a porta de leitura
		 * @param writePort a ports de escrita
		 * @param bit o bit para escrita/leitura
		 * @param direction o endereço do registrador de direção da porta
		 */
		inline Pin(Port readPort, Port writePort, Bit bit, Port direction) : _readPort(readPort), _writePort(writePort),
																	  _bit(bit), _direction(direction) {

		}

	public:
		/**
		 * Define o valor do pino
		 *
		 * @param bit o valor do pino
		 *
		 * @return this
		 */
		inline Pin& operator=(uint8_t bit) {
			*_direction |= (1 << _bit);
			if((bit & 0x01)) {
				*_writePort |= (1 << _bit);
			} else {
				*_writePort &= ~(1 << _bit);
			}
			return *this;
		}

		/**
		 * Converte o bit para um valor inteiro (1 ou 0)
		 *
		 * @return o valor equivalente do pino
		 */
		inline operator uint8_t() const {
			*_direction &= ~(1 << _bit);
			return *_readPort & (1 << _bit);
		}

		/**
		 * Converte o valor do bit para um valor boolean
		 *
		 * @return o valor equivalente do pino
		 */
		inline operator bool() const {
			*_direction &= ~(1 << _bit);
			return *_readPort & (1 << _bit);
		}

	};
}


