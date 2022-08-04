#ifndef BASICMEMORY_BASICMEMORY_HPP
#define BASICMEMORY_BASICMEMORY_HPP

// Credits: https://developer.ibm.com/tutorials/l-memory/

// Include sbrk function
#include <unistd.h>

namespace nBasicMemory {
	struct MemoryControlBlock_t {
		bool   m_bIsAvailable;
		size_t m_uiSize;
	};

	class CBasicMemory {
		public:
			/**
			 * \brief Allocates the requested memory and returns a pointer to it
			 * \param szAllocateBlock This is the size of the memory block, in bytes
			 * \return This function returns a pointer to the allocated memory, or null if the request fails
			 */
			void *Malloc( size_t szAllocateBlock );
			/**
			 * \brief Deallocate the memory previously allocated by a call to calloc, malloc, or realloc
			 * \param pMemoryBlock This is the pointer to a memory block previously allocated with malloc, calloc or realloc to be deallocated. If a null pointer is passed as argument, no action occurs
			 */
			static void Free( void *pMemoryBlock );

		private:
			/**
			 * \brief A function that will search for current break and initialize variables
			 */
			void MallocInitialize( );

			/**
			 * \brief Was the MallocInitialize( ) function called?
			 */
			bool m_bIsInitialized = false;

			void *m_pManagedMemoryStart = nullptr;
			void *m_pLastValidAddress   = nullptr;
	};
}

inline nBasicMemory::CBasicMemory g_BasicMemory;

#include "Implement/BasicMemory.inl"

#endif // BASICMEMORY_BASICMEMORY_HPP