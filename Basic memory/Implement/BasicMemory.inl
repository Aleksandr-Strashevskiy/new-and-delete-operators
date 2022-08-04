#ifndef BASICMEMORY_IMPLEMENT_BASICMEMORY_HPP
#define BASICMEMORY_IMPLEMENT_BASICMEMORY_HPP

#include "../BasicMemory.hpp"

inline void nBasicMemory::CBasicMemory::MallocInitialize( ) {

	// Dont need initialize twice
	if ( m_bIsInitialized )
		return;

	// Grab the last valid address from the OS
	m_pLastValidAddress = sbrk( 0 );

	// We don't have any memory to manage yet, so just set the beginning to be m_pLastValidAddress
	m_pManagedMemoryStart = m_pLastValidAddress;

	// Initializing successful
	m_bIsInitialized = true;
}

#endif // BASICMEMORY_IMPLEMENT_BASICMEMORY_HPP