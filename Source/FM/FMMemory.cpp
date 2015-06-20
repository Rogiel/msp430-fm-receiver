//
//  Juice - Peer to Peer Networking library
//
//  Created by Rogiel Sulzbach.
//  Copyright (c) 2014-2015 Rogiel Sulzbach. All rights reserved.
//

#include "FMMemory.h"

namespace FM {
	/**
	 * Aloca o object FMMemory compartilhado na seção ".text" (memória flash interna)
	 */
	FMMemory FMMemory::_sharedMemory __attribute__ ((section (".text")));
}