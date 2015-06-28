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

#include "IO/Pin.h"
#include "IO/Button.h"
#include "IO/FlashVariable.h"

#include "I2C/I2CMaster.h"

#include "FM/FMReceiver.h"
#include "FM/FMMemory.h"

class Application {
private:
	/**
	 * A instância do bus I2C para este aplicativo
	 */
	I2C::I2CMaster _bus;

	/**
	 * A instância do receptor FM para este aplicativo
	 */
	FM::FMReceiver _receiver;

private:
	/**
	 * A instância do botão de salvar estação
	 */
	IO::DebouncedButton _saveButton;

	/**
	 * A instância do botão de apagar estação
	 */
	IO::DebouncedButton _eraseButton;

	/**
	 * A instância do botão de mudar de estação na memória
	 */
	IO::DebouncedButton _changeMemoryButton;

	/**
	 * A instância do botão de mudar a frequência para cima
	 */
	IO::DebouncedButton _upButton;

	/**
	 * A instância do botão de mudar a frequência para baixo
	 */
	IO::DebouncedButton _downButton;

	/**
	 * A instância do botão de mudo
	 */
	IO::DebouncedButton _muteButton;

private:
	FM::FMMemory::StationIndex _currentIndex;

public:
	/**
	 * Cria uma nova instância do aplicativo
	 */
	Application();

public:
	/**
	 * Initializes the application
	 */
	void init();

	/**
	 * Executa o loop principal
	 */
	void loop();

private:
	/**
	 * Detecta e executa o botão pressionado
	 */
	void handleButtons();

	/**
	 * Executa a ação do botão "salvar estação"
	 */
	void handleSaveStation();

	/**
	 * Executa a ação do botão "apagar estação"
	 */
	void handleEraseStation();

	/**
	 * Executa a ação do botão "mudar estação memorizada"
	 */
	void handleChangeMemorizedStation();

	/**
	 * Executa a ação do botão de move o sintonizador para cima
	 */
	void handleMoveUp();

	/**
	 * Executa a ação do botão de move o sintonizador para baixo
	 */
	void handleMoveDown();

	/**
	 * Executa a ação do botão "mudo"
	 */
	void handleMute();

public:
	/**
	 * Retorna uma instância compartilhada do aplicativo
	 */
	static Application& sharedApplication();

public:
	/**
	 * @return a instância do bus I2C
	 */
	I2C::I2CMaster& getBus() {
		return _bus;
	}

	/**
	 * @return a instância do receptor FM
	 */
	FM::FMReceiver& getReceiver() {
		return _receiver;
	}

	/**
	 * @return a instância da memória de rádio FM
	 */
	const FM::FMMemory& getMemory() const;

	void setMemory(const FM::FMMemory& memory);

	/**
	 * @return uma referência ao botão "salvar"
	 */
	const IO::DebouncedButton& getSaveButton() const {
		return _saveButton;
	}

	/**
	 * @return uma referência ao botão "apagar"
	 */
	const IO::DebouncedButton& getEraseButton() const {
		return _eraseButton;
	}

	/**
	 * @return uma referência ao botão "trocar"
	 */
	const IO::DebouncedButton& getChangeMemoryButton() const {
		return _changeMemoryButton;
	}

	/**
	 * @return uma referência ao botão "subir"
	 */
	const IO::DebouncedButton& getUpButton() const {
		return _upButton;
	}

	/**
	 * @return uma referência ao botão "descer"
	 */
	const IO::DebouncedButton& getDownButton() const {
		return _downButton;
	}

	/**
	 * @return uma referência ao botão "mudo"
	 */
	const IO::DebouncedButton& getMuteButton() const {
		return _muteButton;
	}
};
