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

#include "Application.h"

namespace {
	constexpr uint8_t BUTTON_SAVE_PORT = 1;
	constexpr uint8_t BUTTON_SAVE_BIT  = 0;
	constexpr uint8_t BUTTON_ERASE_PORT = 1;
	constexpr uint8_t BUTTON_ERASE_BIT  = 1;
	constexpr uint8_t BUTTON_CHANGE_PORT = 1;
	constexpr uint8_t BUTTON_CHANGE_BIT  = 2;
	constexpr uint8_t BUTTON_UP_PORT = 1;
	constexpr uint8_t BUTTON_UP_BIT  = 3;
	constexpr uint8_t BUTTON_DOWN_PORT = 1;
	constexpr uint8_t BUTTON_DOWN_BIT  = 4;
	constexpr uint8_t BUTTON_MUTE_PORT = 1;
	constexpr uint8_t BUTTON_MUTE_BIT  = 5;
}

Application::Application() :
		_bus(),
		_receiver(_bus),

		_saveButton(IO::Pin::get<BUTTON_SAVE_PORT>(BUTTON_SAVE_BIT)),
		_eraseButton(IO::Pin::get<BUTTON_ERASE_PORT>(BUTTON_ERASE_BIT)),
		_changeMemoryButton(IO::Pin::get<BUTTON_CHANGE_PORT>(BUTTON_CHANGE_BIT)),
		_upButton(IO::Pin::get<BUTTON_UP_PORT>(BUTTON_UP_BIT)),
		_downButton(IO::Pin::get<BUTTON_DOWN_PORT>(BUTTON_DOWN_BIT)),
		_muteButton(IO::Pin::get<BUTTON_MUTE_PORT>(BUTTON_MUTE_BIT)) {
}

void Application::init() {
	// Para o contador watchdog
	WDTCTL = WDTPW | WDTHOLD;

//	// configura o clock interno para 16 MHz
//	BCSCTL1 = CALBC1_16MHZ;
//	DCOCTL  = CALDCO_16MHZ;

	// ativa o bus
	_bus.enable();
}

void Application::loop() {
	 handleButtons();
	_receiver.updateIfNeeded();
	_bus.wait();
}

void Application::handleButtons() {
	if(_saveButton.isPressed()) {
		handleSaveStation();
	} else if(_eraseButton.isPressed()) {
		handleEraseStation();
	} else if(_changeMemoryButton.isPressed()) {
		handleChangeMemorizedStation();
	} else if(_upButton.isPressed()) {
		handleMoveUp();
	} else if(_downButton.isPressed()) {
		handleMoveDown();
	} else if(_muteButton.isPressed()) {
		handleMute();
	}
}

void Application::handleSaveStation() {
	auto memory = getMemory();

	FM::FMMemory::Station station(_receiver.getFrequency());
	_currentIndex = memory.addStation(station);

	setMemory(memory);
}

void Application::handleEraseStation() {
	auto memory = getMemory();

	memory.removeStation(_currentIndex);

	setMemory(memory);
}

void Application::handleChangeMemorizedStation() {
	auto count = getMemory().getStationCount();
	if(count == 0) {
		return;
	}

	if(count >= _currentIndex) {
		_currentIndex++;
		auto& station = getMemory().getStation(_currentIndex);
		_receiver.setFrequency(station.getFrequency());
	}
}

void Application::handleMoveUp() {
	auto frequency = _receiver.getFrequency();
	frequency += 0.1;

	// limite superior de frequencia no padrão US
	if(frequency > 108.0) {
		frequency = 108.0;
	}

	_receiver.setFrequency(frequency);
}

void Application::handleMoveDown() {
	auto frequency = _receiver.getFrequency();
	frequency -= 0.1;

	// limite inferior de frequencia no padrão US
	if(frequency < 87.8) {
		frequency = 87.8;
	}

	_receiver.setFrequency(frequency);
}

void Application::handleMute() {
	if(_receiver.isMuted()) {
		_receiver.unmute();
	} else {
		_receiver.mute();
	}
}

//namespace {
//	Application _sharedApplication = Application();
//}
//
//Application& Application::sharedApplication() {
//	return _sharedApplication;
//}

namespace {
	DECLARE_FLASH_VARIABLE(FM::FMMemory, _fmMemory);
}

const FM::FMMemory& Application::getMemory() const {
	return _fmMemory;
}

void Application::setMemory(const FM::FMMemory& memory) {
	_fmMemory = memory;
}