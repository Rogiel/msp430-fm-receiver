//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include "FMReceiver.h"

namespace FM {
	FMReceiver::FMReceiver(I2C::I2CBus& bus) : _bus(bus), _needsUpdate(true) {

	}

	FMReceiver::Frequency FMReceiver::getFrequency() const {
		return _frequency;
	}

	void FMReceiver::setFrequency(Frequency frequency) {
		_frequency = frequency;
		setNeedsUpdate();
	}

	FMReceiver::Volume FMReceiver::getVolume() const {
		return _volume;
	}

	void FMReceiver::setVolume(Volume volume) {
		_volume = volume;
		setNeedsUpdate();
	}

	void FMReceiver::increaseVolume() {
		auto volume = getVolume() + 0.01;
		if(volume >= 1.0) {
			volume = 1.0;
		}
		setVolume(volume);
	}

	void FMReceiver::decreaseVolume() {
		auto volume = getVolume() - 0.01;
		if(volume <= 0.0) {
			volume = 0.0;
		}
		setVolume(volume);
	}

	inline void FMReceiver::setNeedsUpdate() {
		_needsUpdate = true;
	}

	void FMReceiver::updateIfNeeded() {
		if(!_needsUpdate) return;
		update();
	}

	void FMReceiver::update() {
		// TODO implement real code here
		_bus.start();

		_bus.transmit(I2C_ADDRESS);
		_bus.transmit(0x2E);
		_bus.transmit(0xBA);
		_bus.transmit(0x10);
		_bus.transmit(0x10);
		_bus.transmit(0xC0);

		_bus.stop();

		_needsUpdate = false;
	}
}
