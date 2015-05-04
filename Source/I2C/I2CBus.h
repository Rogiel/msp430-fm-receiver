//
//  Juice - Peer to Peer Networking library
//
//  Created by Rogiel Sulzbach.
//  Copyright (c) 2014-2015 Rogiel Sulzbach. All rights reserved.
//

#pragma once

#include <msp430.h>
#include <stdint.h>

namespace I2C {
	class I2CBus {
	public:
		typedef uint8_t* Port;

	private:
		/**
		 * Define a porta de saída utilizada para o SCL
		 */
		Port _sclOut;

		/**
		 * Define a porta de entrada utilizada para o SCL
		 */
		Port _sclIn;

		/**
		 * Define a porta de saída utilizada para o SDA
		 */
		Port _sdaOut;

		/**
		 * Define a porta de entrada utilizada para o SDA
		 */
		Port _sdaIn;

	public:
		/**
		 * Cria um novo bus I2C
		 *
		 * @param scl a porta SCL
		 * @param sda a porta SDA
		 */
		I2CBus(Port sclOut, Port sclIn, Port sdaOut, Port sdaIn);

	public:
		void start();
		void stop();

		uint8_t receive(uint8_t ack);
		uint8_t transmit(uint8_t data);

	private:
		void writeSDA(uint8_t data);
		uint8_t readSDA();

		void writeSCL(uint8_t data);
		uint8_t readSCL();

		void delay() const;
	};
}