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

#include "../I2C/I2CBus.h"

namespace FM {
	class FMReceiver {
	public:
		/**
		 * O tipo de dados da frequencia FM
		 */
		typedef float Frequency;

		/**
		 * O endereço I2C do chip FM
		 */
		static const char I2C_ADDRESS = 0b01100000;

	private:
		/**
		 * A frequencia central FM
		 */
		Frequency _frequency;

		/**
		 * True se o receptor está em mudo
		 */
		bool _mute;

		/**
		 * Uma referencia ao objeto do bus I2C
		 */
		I2C::I2CBus& _bus;

		/**
		 * Uma flag indicando se o receptor precisa ser atualizado
		 */
		bool _needsUpdate;

	public:
		/**
		 * Cria um novo receptor FM
		 *
		 * @param bus o bus I2C
		 */
		FMReceiver(I2C::I2CBus& bus);

	public:

		/**
		 * Retorna a frequência central do FM
		 *
		 * @return a frequencia central do FM
		 */
		Frequency getFrequency() const;

		/**
		 * Seta a frequencia central do FM
		 *
		 * @param frequency a frequencia central FM
		 */
		void setFrequency(Frequency frequency);

		/**
		 * Check if the radio is currently muted
		 *
		 * @return true if the received is muted
		 */
		bool isMuted() const;

		/**
		 * Coloca o receptor em mudo
		 */
		void mute();

		/**
		 * Retira o mudo no receptor
		 */
		void unmute();

	private:
		/**
		 * Requisita uma atualização dos dados no chip
		 */
		void setNeedsUpdate();

	public:
		/**
		 * Atualiza os dados do chip se necessário.
		 */
		void updateIfNeeded();

		/**
		 * Atualiza os dados do chip.
		 */
		void update();

	private:
		/**
		 * Calcula a palavra de sintonia do PLL.
		 *
		 * @note os últimos 2 bits da palavra são nulos.
		 * @return uma palavra de 14 bits para sintonia do PLL
		 */
		uint16_t calculatePLLWord() const;

	};
}

