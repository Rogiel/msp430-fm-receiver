//
//  Juice - Peer to Peer Networking library
//
//  Created by Rogiel Sulzbach.
//  Copyright (c) 2014-2015 Rogiel Sulzbach. All rights reserved.
//

#pragma once

#include <msp430.h>
#include <stdint.h>

namespace FM {
	class FMMemory {
	public:
		class Station {
		public:
			typedef uint16_t PLLSwitches;

		public:
			PLLSwitches _switches;
		};

	public:
		Station _memory[20];

	public:
		void addStation() {

		}


	private:
		static FMMemory _sharedMemory;

	public:
		static FMMemory& sharedMemory() {
			return _sharedMemory;
		}

	};
}


