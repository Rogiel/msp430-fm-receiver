//
//  Juice - Peer to Peer Networking library
//
//  Created by Rogiel Sulzbach.
//  Copyright (c) 2014-2015 Rogiel Sulzbach. All rights reserved.
//

#include "FMMemory.h"

namespace FM {

	FMMemory::Station::Station() = default;
	FMMemory::Station::Station(PLLSwitches switches) : _switches(switches) {}

	FMMemory::Station::Station(const Station& station) = default;
	FMMemory::Station& FMMemory::Station::operator=(const Station& station) = default;

	FMMemory::Station::PLLSwitches FMMemory::Station::getPLLSwitches() const {
		return _switches;
	}

	void FMMemory::Station::setPLLSwitches(PLLSwitches pllSwitches) {
		_switches = pllSwitches;
	}

	FMMemory::StationIndex FMMemory::getStationCount() {
		return _count;
	}

	const FMMemory::Station& FMMemory::getStation(StationIndex n) const {
		return _memory[n];
	}

	FMMemory::StationIndex FMMemory::addStation(const Station& station) {
		// memória cheia!
		if(_count == 20) return -1;

		_memory[_count] = station;
		return _count++;
	}

	void FMMemory::removeStation(StationIndex position) {
		if(position >= _count) return;
		if(position >= 20) return;

		for(int i = position; i<_count-1; i++) {
			_memory[i] = _memory[i+1];
		}

		_count--;
	}

}