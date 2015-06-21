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

#include "IO/Button.h"
#include "IO/FlashVariable.h"

#include "I2C/I2CBus.h"

#include "FM/FMReceiver.h"
#include "FM/FMMemory.h"

class Application {
private:
	/**
	 * A instância do bus I2C para este aplicativo
	 */
	I2C::I2CBus bus;

	/**
	 * A instância do receptor FM para este aplicativo
	 */
	FM::FMReceiver receiver;

private:
	/**
	 * A instância do botão de salvar estação
	 */
	IO::DebouncedButton saveButton;

	/**
	 * A instância do botão de apagar estação
	 */
	IO::DebouncedButton eraseButton;

	/**
	 * A instância do botão de mudar de estação na memória
	 */
	IO::DebouncedButton changeMemoryButton;

	/**
	 * A instância do botão de mudar a frequência para cima
	 */
	IO::DebouncedButton upButton;

	/**
	 * A instância do botão de mudar a frequência para baixo
	 */
	IO::DebouncedButton downButton;

	/**
	 * A instância do botão de mudo
	 */
	IO::DebouncedButton muteButton;

public:
	/**
	 * Cria uma nova instância do aplicativo
	 */
	Application();

public:
	/**
	 * Executa o loop principal
	 *
	 * @return o valor de retorno
	 */
	int run();

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
	I2C::I2CBus& getBus() {
		return bus;
	}

	/**
	 * @return a instância do receptor FM
	 */
	FM::FMReceiver& getReceiver() {
		return receiver;
	}

	/**
	 * @return a instância da memória de rádio FM
	 */
	const FM::FMMemory& getMemory() const;

	/**
	 * @return uma referência ao botão "salvar"
	 */
	const IO::DebouncedButton& getSaveButton() const {
		return saveButton;
	}

	/**
	 * @return uma referência ao botão "apagar"
	 */
	const IO::DebouncedButton& getEraseButton() const {
		return eraseButton;
	}

	/**
	 * @return uma referência ao botão "trocar"
	 */
	const IO::DebouncedButton& getChangeMemoryButton() const {
		return changeMemoryButton;
	}

	/**
	 * @return uma referência ao botão "subir"
	 */
	const IO::DebouncedButton& getUpButton() const {
		return upButton;
	}

	/**
	 * @return uma referência ao botão "descer"
	 */
	const IO::DebouncedButton& getDownButton() const {
		return downButton;
	}

	/**
	 * @return uma referência ao botão "mudo"
	 */
	const IO::DebouncedButton& getMuteButton() const {
		return muteButton;
	}
};
