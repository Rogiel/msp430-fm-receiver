//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include "I2CBus.h"

namespace I2C {
	I2CBus::I2CBus(IO::Pin scl, IO::Pin sda) : _scl(scl), _sda(sda) {
		_scl = 0;
		_sda = 0;

		_scl.pullup(true);
		_sda.pullup(true);
	}

	I2CBus::~I2CBus() {
		_sda = 0;
		_scl = 0;
	}

	void I2CBus::start() {
		writeSDA(0x01);
		writeSCL(0x01);

		writeSDA(0x00);
		writeSCL(0x00);
	}

	void I2CBus::stop() {
		writeSDA(0x00);
		writeSCL(0x01);
		writeSDA(0x01);
	}

	uint8_t I2CBus::receive(bool ack) {
		uint8_t bit;
		uint8_t read = 0;

		_sda = 1;
		for(bit = 0; bit <8; bit++) {
			read <<= 1;
			do {
				writeSCL(1);
			}
			while(readSCL() == 0);    // wait for any SCL clock stretching
			delay();
			if(readSDA()) read |= 1;
			_scl = 0;
		}

		if(ack) {
			_sda = 0;
		} else {
			_sda = 1;
		}
		_scl = 1;
		delay();             // send (N)ACK bit
		_scl = 0;
		_sda = 1;

		return read;
	}

	bool I2CBus::transmit(uint8_t data) {
		char bit;
		bool ack;

		for(bit = 8; bit; bit--) {
			if(data & 0x80) {
				_sda = 1;
			} else {
				_sda = 0;
			}
			_scl = 1;
			data <<= 1;
			delay();
			_scl = 0;
		}
		_sda = 1;
		_scl = 1;
		delay();

		ack = _sda;          // possible ACK bit
		_scl = 0;

		return ack;
	}

	inline void I2CBus::writeSDA(uint8_t data) {
		_sda = data;
		delay();
	}

	inline uint8_t I2CBus::readSDA() {
		return _sda;
	}

	inline void I2CBus::writeSCL(uint8_t data) {
		_scl = data;
		delay();
	}

	inline uint8_t I2CBus::readSCL() {
		return _scl;
	}

	void I2CBus::delay() const {
		__delay_cycles(100000);
		// no-op
	}
}