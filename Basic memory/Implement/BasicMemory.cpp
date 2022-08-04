#include "../BasicMemory.hpp"

void *nBasicMemory::CBasicMemory::Malloc( size_t szAllocateBlock ) {

	// Initialize memory allocator
	MallocInitialize( );

	// The memory we search for has to include the memory control block, but the users of malloc don't need to know this, so we'll just add it in for them
	szAllocateBlock += sizeof( MemoryControlBlock_t );

	// Set pMemoryLocation to null until we find a suitable location
	void *pMemoryLocation = nullptr;

	// Begin searching at the start of managed memory
	void *pCurrentLocation = m_pManagedMemoryStart;

	// This is the same as pCurrentLocation, but cast to MemoryControlBlock_t
	MemoryControlBlock_t *pCurrentLocationBlock;

	// Keep going until we have searched all allocated space
	while ( pCurrentLocation != m_pLastValidAddress ) {

		/* pCurrentLocation and pCurrentLocationBlock point to the same address.
		 * However, pCurrentLocationBlockis of the correct type, so we can use it as a struct.
		 * pCurrentLocation is a void pointer so we can use it to calculate addresses
		 */

		pCurrentLocationBlock = static_cast< MemoryControlBlock_t * >( pCurrentLocation );

		if ( pCurrentLocationBlock->m_bIsAvailable ) {

			if ( pCurrentLocationBlock->m_uiSize >= szAllocateBlock ) {

				// We've found an open, appropriately size location

				// It is no longer available
				pCurrentLocationBlock->m_bIsAvailable = false;

				// We own it
				pMemoryLocation = pCurrentLocation;

				// Leave the loop
				break;
			}
		}

		// If we made it here, it's because the current memory block not suitable; move to the next one
		pCurrentLocation = static_cast< char * >( pCurrentLocation )
				+ static_cast< int >( pCurrentLocationBlock->m_uiSize );
	}

	// If we still don't have a valid location, we'll have to ask the operating system for more memory
	if ( pMemoryLocation == nullptr ) {

		// Move the program break szMemoryBlock further
		sbrk( static_cast< int >( szAllocateBlock ) );

		// The new memory will be where the last valid address left off
		pMemoryLocation = m_pLastValidAddress;

		// We'll move the last valid address forward szMemoryBlock
		m_pLastValidAddress = static_cast< char * >( m_pLastValidAddress )
				+ static_cast< int >( szAllocateBlock );

		// We need to initialize the MemoryControlBlock_t
		pCurrentLocationBlock = static_cast< MemoryControlBlock_t * >( pMemoryLocation );

		pCurrentLocationBlock->m_bIsAvailable = false;
		pCurrentLocationBlock->m_uiSize       = szAllocateBlock;
	}

	/*
	 * Now, no matter what (well, except for error conditions),
	 * pMemoryLocation has the address of the memory, including MemoryControlBlock_t
	 */

	// Move the pointer past the MemoryControlBlock_t
	pMemoryLocation = static_cast< char * >( pMemoryLocation ) + sizeof( MemoryControlBlock_t );

	// Return the pointer
	return pMemoryLocation;
}

void nBasicMemory::CBasicMemory::Free( void *pMemoryBlock ) {

	// Backup from the given pointer to find the MemoryControlBlock_t
	const auto pControlBlock = reinterpret_cast< MemoryControlBlock_t * >(
		static_cast< char * >( pMemoryBlock ) - sizeof( MemoryControlBlock_t ) );

	// Mark the block as being available
	pControlBlock->m_bIsAvailable = true;
}