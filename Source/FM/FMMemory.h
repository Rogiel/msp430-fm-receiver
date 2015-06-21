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

#include <msp430.h>
#include <stdint.h>

#include "FMReceiver.h"

namespace FM {
	/**
	 * Objeto que controla a memória FM. É recomendado que este objeto seja armazenado na memória flash.
	 */
	class FMMemory {
	public:
		/**
		 * Constante indicando o número máximo de estações na memória
		 */
		static constexpr uint16_t MAX_MEMORY_STATIONS = 20;

	public:
		/**
		 * Define o objeto que representa uma estação na memória
		 */
		class Station {
		public:
			/**
			 * Alias de tipo de frequência
			 */
			typedef FMReceiver::Frequency Frequency;

		private:
			/**
			 * A frequência da estação salva na memória
			 */
			Frequency _frequency;

		public:
			/**
			 * Cria uma estação vazia
			 */
			Station();

			/*
			 * Cria uma estação com uma frequencia
			 *
			 * @param frequency a frequencia da estação FM
			 */
			Station(Frequency frequency);

			/**
			 * Construtor de cópia
			 *
			 * @param other a instância para copiar
			 */
			Station(const Station& station);

			/**
			 * Operator de cópia
			 *
			 * @param other a instância para copiar
			 * @return this
			 */
			Station& operator=(const Station& station);

		public:
			/**
			 * Retorna a frequência da estação
			 *
			 * @return a frequência da estação em MHz
			 */
			Frequency getFrequency() const;

			/**
			 * Define a frequencia da estação
			 *
			 * @param frequency a nova frequência em MHz
			 */
			void setFrequency(Frequency frequency);
		};

		/**
		 * Define o tipo usado para indexar as estações na memória
		 */
		typedef int8_t StationIndex;

	private:
		/**
		 * O número de estações atualmente armazenadas na memória
		 */
		StationIndex _count;

		/**
		 * A array que contém a memória de estações
		 */
		Station _memory[MAX_MEMORY_STATIONS];

	public:
		/**
		 * Cria uma nova instância da FM memory
		 */
		FMMemory() : _count(0) {}

		/**
		* Construtor de cópia
	    *
		* @param other a instância para copiar
	    */
		FMMemory(const FMMemory& other) = default;

		/**
		 * Operator de cópia
		 *
		 * @param other a instância para copiar
		 * @return this
		 */
		FMMemory& operator=(const FMMemory& other) = default;

	public:
		/**
		 * @return o número total de estações salvas na memória
		 */
		StationIndex getStationCount();

		/**
		 * Retorna uma estação pelo seu índice
		 *
		 * @param n o indice da estação para retornar
		 * @return uma referência constante da estacão encontrada
		 */
		const Station& getStation(StationIndex n) const;

		/**
		 * Adiciona uma estação.
		 *
		 * @param station a estação para ser adicionada
		 * @return o indice da estação adicionada. Retorna -1 se a memória está cheia.
		 */
		StationIndex addStation(const Station& station);

		/**
		 * Remove uma estação da memória
		 *
		 * @param position o indice da estação a ser removida
		 */
		void removeStation(StationIndex position);
	};
}


