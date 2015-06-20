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

#include "../I2C/I2CBus.h"

namespace FM {
	class FMReceiver {
	public:
		typedef float Frequency;
		typedef float Volume;

		static const char I2C_ADDRESS = 0b01100000;

	private:
		Frequency _frequency;
		Volume _volume;

		I2C::I2CBus& _bus;

		bool _needsUpdate;

	public:
		FMReceiver(I2C::I2CBus& bus);

	public:

		Frequency getFrequency() const;
		void setFrequency(Frequency frequency);

		Volume getVolume() const;
		void setVolume(Volume volume);

		void increaseVolume();
		void decreaseVolume();

	private:
		void setNeedsUpdate();

	public:
		void updateIfNeeded();
		void update();

	};
}

