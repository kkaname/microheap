#include "microheap.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

#define ALIGNMENT 32
typedef unsigned char uint8_t;
#define ALIGN16(x) (((((x) - 1) >> 4) << 4) + 16)

/*
 * this is the actual allocated block, where
 * the header is hidder from the calling function
 * ,------------------------.
 * |        | Actual memory |
 * | header |     block     |
 * |        |               |
 * '------------------------'
 *  ^
 *  '-- this header is hidden from the calling function
 */
typedef union memory_block_header {
    struct {
        size_t size;
        uint8_t is_free;
        union memory_block_header *next;
    } s;
    char align[ALIGNMENT];
} header_t;


header_t *head = NULL;
header_t *tail = NULL;

header_t *get_free_block(size_t size) {
    header_t *curr_ptr = head;
    while(curr_ptr) {
        // to check whether a block is free and has enough size
        if (curr_ptr->s.is_free && curr_ptr->s.size >= size) {
            return curr_ptr;
        }
        curr_ptr = curr_ptr->s.next;
    }
    return NULL;
}

void *my_malloc(size_t size) {
    if (!size) {
        return NULL;
    }
    size_t aligned_size = ALIGN16(size);
    size_t total_size = aligned_size + sizeof(header_t);
    void *block = NULL;
    header_t *header = NULL;

    pthread_mutex_lock(&global_malloc_lock);

    header = get_free_block(aligned_size);
    if (header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void *)(header + 1);
    }

    header = sbrk(total_size);
    if (header == (void *) -1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;

    if(!head) {
        head = tail= header;
    }
    else {
        tail->s.next = header;
        tail = header;
    }
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)((header_t *)header + 1);
}

 // here if the block is at the end of the heap, then we realease
 // it to the OS, else we mark it as free
void my_free(void *block) {
    if (!block) {
        return;
    }
    pthread_mutex_lock(&global_malloc_lock);

    header_t *header = (header_t *)block - 1;
    header_t *tmp;
    void *program_break = sbrk(0);

    if ((char *)block + header->s.size == program_break) {
        if (head == tail) {
            head = tail = NULL;
        }
        else {
            tmp = head;
            while (tmp) {
                if (tmp->s.next == tail) {
                    tmp->s.next = NULL;
                    tail = tmp;
                    break;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - sizeof(header_t) + header->s.size);
        pthread_mutex_unlock(&global_malloc_lock);
        // printf("Block freed successfully\n");
        return;
    }
    // header->s.size = 0;
    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
    // printf("Block marked as free\n");
    return;
}

void *my_calloc(size_t num, size_t nsize) {
    if (!num || !nsize) {
        return NULL;
    }
    size_t total_size = num * nsize;
    if (nsize != total_size / num) {
        // to check MUl overflow
        return NULL;
    }

    void *block = my_malloc(total_size);
    if (!block) {
        return NULL;
    }
    memset(block, 0, total_size);
    return block;
}

void *my_realloc(void *block, size_t size) {
    if (!block) {
        return my_malloc(size);
    }
    if (!size) {
        my_free(block);
        return NULL;
    }

    header_t *header;
    void *ret;
    header = (header_t *)block - 1;
    if (header->s.size >= size) {
        return block;
    }

    ret = my_malloc(size);
    if (ret) {
        //reallocating contents to the new bigger block
        memcpy(ret, block, header->s.size);
        my_free(block);
    }
    return ret;
}
