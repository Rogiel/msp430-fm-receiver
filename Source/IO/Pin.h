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
		 * O endereço do registrador de controle do resistor de pull-up interno
		 */
		Port _pullup;

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
		 * @param pullup o endereço do registrador pullup
		 */
		inline Pin(Port readPort, Port writePort, Bit bit, Port direction, Port pullup) :
				_readPort(readPort), _writePort(writePort), _bit(bit), _direction(direction), _pullup(pullup) { }

	public:
		/**
		 * Define o valor do pino
		 *
		 * @param bit o valor do pino
		 *
		 * @return this
		 */
		inline Pin& operator=(uint8_t bit) {
			output();
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
			input();
			return (uint8_t) (*_readPort & (1 << _bit) & 0x01);
		}

		/**
		 * Compara o valor do pino com um valor inteiro
		 *
		 * @param b o valor inteiro para comparar
		 * @return true se os valores são equivalentes
		 */
		inline bool operator==(uint8_t i) const {
			input();
			return (*_readPort & (1 << _bit)) == i;
		}

		/**
		 * Converte o valor do bit para um valor boolean
		 *
		 * @return o valor equivalente do pino
		 */
		inline operator bool() const {
			input();
			return (bool) *_readPort & (1 << _bit);
		}

		/**
		 * Compara o valor do pino com um valor boolean
		 *
		 * @param b o valor boolean para comparar
		 * @return true se os valores são equivalentes
		 */
		inline bool operator==(const bool b) const {
			input();
			return (*_readPort & (1 << _bit)) == b;
		}

	public:
		inline void input() const {
			*_direction &= ~(1 << _bit);
		}

		inline void output() {
			*_direction |= (1 << _bit);
		}

		inline void pullup(bool state) {
			if(state) {
				*_pullup |= (1 << _bit);
			} else {
				*_pullup &= ~(1 << _bit);
			}
		}

	public:
		/**
		 * Template não definido para auxiliar a declaração de portas padrões
		 *
		 * @tparam Port a porta para usar
		 * @param bit o bit para ser usado na porta "Port"
		 *
		 * @return o Pin respectivo a porta e ao bit fornecidos
		 */
		template<uint8_t Port>
		static Pin get(uint8_t bit);
	};

#ifdef __MSP430_HAS_PORT1_R__
	/**
	 * Especialização do template para a porta 1
	 *
	 * @param bit o bit para ser usado na porta 1
	 *
	 * @return o Pin respectivo a porta 1 e ao bit fornecido
	 */
	template<>
	inline Pin Pin::get<1>(uint8_t bit) {
		return Pin((IO::Pin::Port) &P1IN, (IO::Pin::Port) &P1OUT, bit, (IO::Pin::Port) &P1DIR, (IO::Pin::Port) &P1REN);
	}
#endif

#ifdef __MSP430_HAS_PORT2_R__
	/**
	 * Especialização do template para a porta 2
	 *
	 * @param bit o bit para ser usado na porta 2
	 *
	 * @return o Pin respectivo a porta 2 e ao bit fornecido
	 */
	template<>
	inline Pin Pin::get<2>(uint8_t bit) {
		return Pin((IO::Pin::Port) &P2IN, (IO::Pin::Port) &P2OUT, bit, (IO::Pin::Port) &P2DIR, (IO::Pin::Port) &P2REN);
	}
#endif

#ifdef __MSP430_HAS_PORT3_R__
	/**
	 * Especialização do template para a porta 3
	 *
	 * @param bit o bit para ser usado na porta 3
	 *
	 * @return o Pin respectivo a porta 3 e ao bit fornecido
	 */
	template<>
	inline Pin Pin::get<3>(uint8_t bit) {
		return Pin((IO::Pin::Port) &P3IN, (IO::Pin::Port) &P3OUT, bit, (IO::Pin::Port) &P3DIR, (IO::Pin::Port) &P3REN);
	}
#endif
}


