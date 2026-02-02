#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t global_malloc_lock;

typedef char align[16];

// this header for each allocated block
// stores the size of a block and info that whether the block of memory is free or not
typedef union memory_block_header {
	struct {
		size_t size;
		unsigned is_free;
		struct header_t *next;	// to make sure that each b
					// allocated block is contiguous
	} s;
	align stub;
} header_t;
/*
 * this is the actual allocated block, where
 * the header is hidden from the calling function
 * ,--------------------,
 * |	  |Actual memory|
 * |header|    block	|
 * |      |             |
 * '--------------------'
 */

header_t *head;
header_t *tail;
// these head and tail pointer is to keep track of the list

void *get_free_block(size_t size) {
    header_t *curr_ptr = head;
    while(curr_ptr) {
        if (curr_ptr->s.is_free && curr_ptr->s.size >= size) {
            return curr_ptr;
        }
        curr_ptr = curr_ptr.s->next;
    }
    return NULL;
}


void *malloc(size_t size)  {
    if (!size)
        return NULL;

	size_t total_size; // total size = size of the block + size of the header
    total_size = size + sizeof(header_t);

	void *block = NULL;
	header_t *header;

	pthread_mutex_lock(&global_malloc_lock);


    header = get_free_block(total_size);

    if (!header) {

    }



	block = sbrk(size);   //returns old program break (before change)
	if (block == (void *)-1) {
		return NULL;
    	}
    	return block;
}

int main(void) {

    return 0;
}
