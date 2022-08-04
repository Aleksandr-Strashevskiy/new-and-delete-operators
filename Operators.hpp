#ifndef OPERATORS_HPP
#define OPERATORS_HPP

// Used: std::bad_alloc
#include <new>

// Used malloc, free
#include "Basic memory/BasicMemory.hpp"

// Note: No inlining, required to replaced functions

/**
 * \brief Allocates requested number of bytes
 * \param szAllocateBlock Number of bytes to allocate
 * \return Pointer to a memory area to initialize the object at
 */
void *operator new( size_t szAllocateBlock ) {

	// Check if allocate blocks null, to avoid call malloc function with zero argument, which may return nullptr on success
	if ( szAllocateBlock == 0U )
		++szAllocateBlock;

	// Allocates the requested memory and returns a pointer to it
	if ( const auto pAllocateMemory = g_BasicMemory.Malloc( szAllocateBlock ) )
		return pAllocateMemory;

	// Thrown as exceptions by the allocation functions to report failure to allocate storage
	throw std::bad_alloc{ };
}

void *operator new[]( const size_t szAllocateBlock ) {

	return operator new( szAllocateBlock );
}

/**
 * \brief Deallocate storage previously allocated by a matching operator new
 * \param pAllocatedMemory Pointer to a memory area to initialize the object at
 */
void operator delete( void *pAllocatedMemory ) {

	// Check if the allocated memory is not equal to null
	if ( pAllocatedMemory != nullptr )
		// Deallocate allocated memory
		nBasicMemory::CBasicMemory::Free( pAllocatedMemory );
}

void operator delete[]( void *pAllocateMemory ) {

	operator delete( pAllocateMemory );
}
#endif // OPERATORS_HPP