//
//  Juice - Peer to Peer Networking library
//
//  Created by Rogiel Sulzbach.
//  Copyright (c) 2014-2015 Rogiel Sulzbach. All rights reserved.
//

#include "I2CBus.h"

namespace I2C {
	I2CBus::I2CBus(Port sclOut, Port sclIn, Port sdaOut, Port sdaIn) : _sclOut(sclOut), _sdaOut(sdaOut) {}

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

	uint8_t I2CBus::receive(uint8_t ack) {
		uint8_t bit;
		uint8_t read = 0;

		*_sdaOut = 1;
		for(bit = 0; bit <8; bit++) {
			read <<= 1;
			do {
				writeSCL(1);
			}
			while(readSCL() == 0);    // wait for any SCL clock stretching
			delay();
			if(readSDA()) read |= 1;
			_sclOut = 0;
		}
		if(ack) {
			*_sdaOut = 0;
		} else {
			*_sdaOut = 1;
		}
		*_sclOut = 1;
		delay();             // send (N)ACK bit
		*_sclOut = 0;
		*_sdaOut = 1;

		return read;
	}

	uint8_t I2CBus::transmit(uint8_t data) {
		char bit;
		static uint8_t ack;
		for(bit = 8; bit; bit--) {
			if(data&0x80) {
				*_sdaOut = 1;
			}
			else {
				*_sdaOut = 0;
			}
			*_sclOut = 1;
			data <<= 1;
			*_sclOut = 0;
		}
		*_sdaOut = 1;
		*_sclOut = 1;
		delay();
		ack = *_sdaIn;          // possible ACK bit
		*_sclOut = 0;

		return ack;
	}

	inline void I2CBus::writeSDA(uint8_t data) {
		*_sdaOut = data;
		delay();
	}

	inline uint8_t I2CBus::readSDA() {
		return *_sdaIn;
	}

	inline void I2CBus::writeSCL(uint8_t data) {
		*_sclOut = data;
		delay();
	}

	inline uint8_t I2CBus::readSCL() {
		return *_sclIn;
	}

	void I2CBus::delay() const {
		__delay_cycles(1000000);
		// no-op
	}
}