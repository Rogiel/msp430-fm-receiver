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

#include "FMMemory.h"

namespace FM {

	FMMemory::Station::Station() = default;
	FMMemory::Station::Station(Frequency frequency) : _frequency(frequency) {}

	FMMemory::Station::Station(const Station& station) = default;
	FMMemory::Station& FMMemory::Station::operator=(const Station& station) = default;

	FMMemory::Station::Frequency FMMemory::Station::getFrequency() const {
		return _frequency;
	}

	void FMMemory::Station::setFrequency(Frequency frequency) {
		_frequency = frequency;
	}

	FMMemory::StationIndex FMMemory::getStationCount() {
		return _count;
	}

	const FMMemory::Station& FMMemory::getStation(StationIndex n) const {
		return _memory[n];
	}

	FMMemory::StationIndex FMMemory::addStation(const Station& station) {
		// memória cheia!
		if(_count == 20) return -1;

		_memory[_count] = station;
		return _count++;
	}

	void FMMemory::removeStation(StationIndex position) {
		if(position >= _count) return;
		if(position >= 20) return;

		for(int i = position; i<_count-1; i++) {
			_memory[i] = _memory[i+1];
		}

		_count--;
	}

}