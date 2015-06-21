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
	constexpr uint8_t BUS_SCL_PORT = 1;
	constexpr uint8_t BUS_SCL_BIT  = 0;
	constexpr uint8_t BUS_SDA_PORT = 1;
	constexpr uint8_t BUS_SDA_BIT  = 6;

	constexpr uint8_t BUTTON_SAVE_PORT = 2;
	constexpr uint8_t BUTTON_SAVE_BIT  = 0;
	constexpr uint8_t BUTTON_ERASE_PORT = 2;
	constexpr uint8_t BUTTON_ERASE_BIT  = 1;
	constexpr uint8_t BUTTON_CHANGE_PORT = 2;
	constexpr uint8_t BUTTON_CHANGE_BIT  = 2;
	constexpr uint8_t BUTTON_UP_PORT = 2;
	constexpr uint8_t BUTTON_UP_BIT  = 3;
	constexpr uint8_t BUTTON_DOWN_PORT = 2;
	constexpr uint8_t BUTTON_DOWN_BIT  = 4;
	constexpr uint8_t BUTTON_MUTE_PORT = 2;
	constexpr uint8_t BUTTON_MUTE_BIT  = 5;
}

Application::Application() :
		bus(IO::Pin::get<BUS_SCL_PORT>(BUS_SCL_BIT), IO::Pin::get<BUS_SDA_PORT>(BUS_SDA_BIT)),
		receiver(bus),
		saveButton(IO::Pin::get<BUTTON_SAVE_PORT>(BUTTON_SAVE_BIT)),
		eraseButton(IO::Pin::get<BUTTON_ERASE_PORT>(BUTTON_ERASE_BIT)),
		changeMemoryButton (IO::Pin::get<BUTTON_CHANGE_PORT>(BUTTON_CHANGE_BIT)),
		upButton(IO::Pin::get<BUTTON_UP_PORT>(BUTTON_UP_BIT)),
		downButton(IO::Pin::get<BUTTON_DOWN_PORT>(BUTTON_DOWN_BIT)),
		muteButton(IO::Pin::get<BUTTON_MUTE_PORT>(BUTTON_MUTE_BIT)) {

}

int Application::run() {
	while(true) {
		handleButtons();
		receiver.updateIfNeeded();
	}
}

void Application::handleButtons() {
	if(saveButton.isPressed()) {
		handleSaveStation();
	} else if(eraseButton.isPressed()) {
		handleEraseStation();
	} else if(changeMemoryButton.isPressed()) {
		handleChangeMemorizedStation();
	} else if(upButton.isPressed()) {
		handleMoveUp();
	} else if(downButton.isPressed()) {
		handleMoveDown();
	} else if(muteButton.isPressed()) {
		handleMute();
	}
}

void Application::handleSaveStation() {
	// TODO implement memory saving
}

void Application::handleEraseStation() {
	// TODO implement memory erasing
}

void Application::handleChangeMemorizedStation() {
	// TODO implement memory tuning
}

void Application::handleMoveUp() {
	auto frequency = receiver.getFrequency();
	frequency += 0.1;

	// limite superior de frequencia no padrão US
	if(frequency > 108.0) {
		frequency = 108.0;
	}

	receiver.setFrequency(frequency);
}

void Application::handleMoveDown() {
	auto frequency = receiver.getFrequency();
	frequency -= 0.1;

	// limite inferior de frequencia no padrão US
	if(frequency < 87.8) {
		frequency = 87.8;
	}

	receiver.setFrequency(frequency);
}

void Application::handleMute() {
	if(receiver.isMuted()) {
		receiver.unmute();
	} else {
		receiver.mute();
	}
}

namespace {
	Application _sharedApplication;
}

Application& Application::sharedApplication() {
	return _sharedApplication;
}

namespace {
	DECLARE_FLASH_VARIABLE(FM::FMMemory, _fmMemory);
}

const FM::FMMemory& Application::getMemory() const {
	return _fmMemory;
}