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
		static constexpr uint16_t MAX_MEMORY_STATIONS = 20;

	public:
		class Station {
		public:
			typedef uint16_t PLLSwitches;

		private:
			PLLSwitches _switches;

		public:
			Station();
			Station(PLLSwitches switches);

			Station(const Station& station);
			Station& operator=(const Station& station);

		public:
			PLLSwitches getPLLSwitches() const;
			void setPLLSwitches(PLLSwitches pllSwitches);
		};

		typedef int8_t StationIndex;

	private:
		StationIndex _count;
		Station _memory[MAX_MEMORY_STATIONS];

	public:
		FMMemory() : _count(0) {}

		FMMemory(const FMMemory& other) = default;
		FMMemory& operator=(const FMMemory& other) = default;

	public:
		StationIndex getStationCount();

		const Station& getStation(StationIndex n) const;

		StationIndex addStation(const Station& station);

		void removeStation(StationIndex position);
	};
}


