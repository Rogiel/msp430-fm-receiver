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

#include <msp430.h>
#include <stdint.h>

#include "../IO/Pin.h"

namespace I2C {
	class I2CBus {
	private:
		/**
		 * Define o pino de utilizado para o SCL
		 */
		IO::Pin _scl;

		/**
		 * Define o pino de utilizado para o SDA
		 */
		IO::Pin _sda;

	public:
		/**
		 * Cria um novo bus I2C
		 *
		 * @param scl o pino SCL
		 * @param sda o pino SDA
		 */
		I2CBus(IO::Pin scl, IO::Pin sda);

	public:
		/**
		 * Envia a sequência de START do I2C.
		 * A partir deste ponto, o bus é considerado OCUPADO até uma sequência de stop.
		 */
		void start();

		/**
		 * Envia a sequência de STOP do I2C
		 * A partir deste ponto, o bus é considerado LIVRE.
		 */
		void stop();

		/**
		 * Recebe um byte do bus.
		 *
		 * @param ack envia um sinal de ACK após recebebimento
		 *
		 * @return o byte lido do bus
		 */
		uint8_t receive(uint8_t ack);

		/**
		 * Transmite um byte para o bus
		 *
		 * @param data o byte para ser enviado
		 *
		 * @return o sinal de ack, se recebido
		 */
		uint8_t transmit(uint8_t data);

	private:
		/**
		 * Escreve um bit no SDA
		 */
		void writeSDA(uint8_t data);

		/**
		 * Lê um bit do SDA
		 */
		uint8_t readSDA();

		/**
		 * Escreve um bit no SCL
		 */
		void writeSCL(uint8_t data);

		/**
		 * Lê um bit do SCL
		 */
		uint8_t readSCL();

		/**
		 * Espera alguns ciclos para garantir a propagação do sinal no bus
		 */
		void delay() const;
	};
}