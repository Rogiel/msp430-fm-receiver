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

#include "I2CMaster.h"

namespace I2C {
	namespace {
		I2CMaster* _activeBus;
	}

	I2CMaster::I2CMaster() {
		_transmitBufferSize = 0;
		_transmitBufferPointer = 0;
		_transmitHandler = nullptr;

		_receiveBufferSize = 0;
		_receiveBufferPointer = 0;
		_receiveBufferReadPointer = 0;
		_receiveHandler = nullptr;

		_busy = false;

		// configure os pinos 6 e 7 da porta 1 no controlador USCI_B0
		P1SEL |= BIT6 + BIT7;
		P1SEL2 |= BIT6 + BIT7;

		// ativa o SW reset (inicia uma mudança de configuração)
		UCB0CTL1 |= UCSWRST;

		// configura como I2C master, modo síncrono
		UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;

		// ativa o uso do SMCLK
		UCB0CTL1 = UCSSEL_2 + UCSWRST;

		// configura o divisor de frequência: fSCL = SMCLK/12 = ~100kHz
		UCB0BR0 = 12;
		UCB0BR1 = 0;

		// desativa a flag SW reset, continua operação
		UCB0CTL1 &= ~UCSWRST;

		// se nenhum bus está ativo, automaticamente ativa o bus corrente
		if(_activeBus == nullptr) {
			enable();
		}
	}

	I2CMaster::~I2CMaster() {
		// se este é o bus ativo, desativa
		if(isActive()) {
			disable();
		}
	}

	bool I2CMaster::transmit(DeviceAddress address, Data data, uint8_t size) {
		if(isBusy())
			return false;
		if(size > 64)
			return false;
		if(!isActive())
			return false;

		for(uint8_t i = 0; i < size; i++) {
			_transmitBuffer[_transmitBufferSize] = data[i];
			_transmitBufferSize++;
		}

		UCB0I2CSA = address;

		// cerifica que o STOP já terminou
		if(UCB0CTL1 & UCTXSTP) {
			return false;
		}

		// inicia uma transmissão e START
		UCB0CTL1 |= UCTR + UCTXSTT;
		__bis_SR_register(GIE);

		// seta o bus como ocupado
		_busy = true;

		return true;
	}

	bool I2CMaster::transmit(DeviceAddress address, const char* string) {
		uint8_t size;
		for(uint8_t i = 0; ; i++) {
			if(string[i] == 0x00) {
				size = i;
				break;
			}
		}
		return transmit(address, (Data) string, size);
	}

	void I2CMaster::attachTransmitHandler(TransmitHandler transmitHandler) {
		_transmitHandler = transmitHandler;
	}

	bool I2CMaster::receive(DeviceAddress address, uint8_t size, bool ack) {
		if(isBusy())
			return false;
		if(size > 64)
			return false;
		if(!isActive())
			return false;

		UCB0I2CSA = address;
		_receiveBufferSize = size;
		_receiveBufferPointer = 0;

		// cerifica que o STOP já terminou
		if(UCB0CTL1 & UCTXSTP) {
			return false;
		}

		// inicia a recepção
		UCB0CTL1 |= UCTXSTT;
		__bis_SR_register(GIE);

		// seta o bus como ocupado
		_busy = true;

		return true;
	}

	I2CMaster::DataByte I2CMaster::get() {
		auto b = _receiveBuffer[_receiveBufferReadPointer];
		_receiveBufferReadPointer++;
		if(_receiveBufferPointer == _receiveBufferSize) {
			cleanupReceiveBuffer();
		}
		return b;
	}

	void I2CMaster::attachReceiveHandler(ReceiveHandler receiveHandler) {
		_receiveHandler = receiveHandler;
	}

	inline bool I2CMaster::isBusy() {
		return _busy;
	}

	void I2CMaster::wait() {
		while(isBusy());
	}

	void I2CMaster::onTransmit() {
		// checa por erros e reinicia o controlador, se necessário
		if ((UCB0STAT & UCNACKIFG) != 0) {
			UCB0CTL1 |= UCTXSTP;
			UCB0STAT &= ~UCNACKIFG;

			_busy = false;

			cleanupTransmitBuffer();
			return;
		} else if ((UCB0STAT & UCALIE) != 0) {
			UCB0CTL1 |= UCTXSTP;
			UCB0STAT &= ~UCALIE;

			_busy = false;

			cleanupTransmitBuffer();
			return;
		}


		if(_transmitBufferSize > _transmitBufferPointer) {
			// copia dados do buffer para o registrador
			UCB0TXBUF = _transmitBuffer[_transmitBufferPointer];
			_transmitBufferPointer++;
		} else {
			// envia uma condição STOP
			UCB0CTL1 |= UCTXSTP;

			_busy = false;

			// limpa o bit de interrupção TX
			IFG2 &= ~UCB0TXIFG;

			// invoca o handler de transmissão
			if(_transmitHandler) {
				_transmitHandler();
			}

			cleanupTransmitBuffer();
		}
	}

	void I2CMaster::onReceive() {
		// checa por erros e reinicia o controlador, se necessário
		if ((UCB0STAT & UCNACKIFG) != 0) {
			UCB0CTL1 |= UCTXSTP;
			UCB0STAT &= ~UCNACKIFG;

			_busy = false;

			return;
		} else if ((UCB0STAT & UCALIE) != 0) {
			UCB0CTL1 |= UCTXSTP;
			UCB0STAT &= ~UCALIE;

			_busy = false;

			return;
		}

		if(_receiveBufferSize > _receiveBufferPointer) {
			// copia dados do registrador para o buffer
			_receiveBuffer[_receiveBufferPointer] = UCB0RXBUF;
			_receiveBufferPointer++;
		} else {
			// envia uma condição STOP
			UCB0CTL1 |= UCTXSTP;

			_busy = false;

			// limpa o bit de interrupção TX
			IFG2 &= ~UCB0TXIFG;

			// invoca o handler de recepção
			if(_receiveHandler) {
				_receiveHandler(_receiveBufferSize);
			}
		}
	}

	void I2CMaster::enable() {
		if(_activeBus) {
			_activeBus->disable();
		}

		// seta o bus como ativo
		_activeBus = this;

		// ativa interrupções
		IE2 |= UCB0TXIE | UCB0RXIE;
		__bis_SR_register(GIE);
	}

	void I2CMaster::disable() {
		// desativa as interrupção ao destruir o objeto
		IE2 &= ~(UCB0TXIE | UCB0RXIE);
		_activeBus = nullptr;
	}

	bool I2CMaster::isActive() {
		return _activeBus == this;
	}

	I2CMaster& I2CMaster::activeBus() {
		return *_activeBus;
	}

	void I2CMaster::cleanupReceiveBuffer() {
		_receiveBufferSize = 0;
		_receiveBufferPointer = 0;
		_receiveBufferReadPointer = 0;
	}

	void I2CMaster::cleanupTransmitBuffer() {
		_transmitBufferSize = 0;
		_transmitBufferPointer = 0;
	}

#pragma vector = USCIAB0TX_VECTOR
	__interrupt void USCIAB0TX_ISR(void) {
		if(_activeBus) {
			_activeBus->onTransmit();
		}
	}

#pragma vector = USCIAB0RX_VECTOR
	__interrupt void USCIAB0RX_ISR(void) {
		if(_activeBus) {
			_activeBus->onReceive();
		}
	}

}