#ifndef MICROHEAP_H
#define MICROHEAP_H

#include <stddef.h>

/**
 * MicroHeap - A lightweight thread-safe memory allocator
 * 
 * This library provides custom implementations of standard memory
 * allocation functions with improved control over heap management.
 */

/**
 * Allocates a block of memory of the specified size.
 * 
 * @param size: Number of bytes to allocate
 * @return: Pointer to allocated memory, or NULL on failure
 */
void *my_malloc(size_t size);

/**
 * Frees a previously allocated block of memory.
 * If the block is at the end of the heap, it's returned to the OS.
 * Otherwise, it's marked as free for reuse.
 * 
 * @param block: Pointer to memory block to free
 */
void my_free(void *block);

/**
 * Allocates memory for an array of elements and initializes to zero.
 * 
 * @param num: Number of elements
 * @param nsize: Size of each element
 * @return: Pointer to allocated and zeroed memory, or NULL on failure
 */
void *my_calloc(size_t num, size_t nsize);

/**
 * Resizes a previously allocated memory block.
 * 
 * @param block: Pointer to existing memory block
 * @param size: New size in bytes
 * @return: Pointer to resized memory, or NULL on failure
 */
void *my_realloc(void *block, size_t size);

#endif
