#include "microheap.h"

#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

typedef char align[16];
#define ALIGN16(x) (((x) + 15) & ~15)

/*
 * this is the actual allocated block, where
 * the header is hidder from the calling function
 * ,------------------------.
 * |        | Actual memory |
 * | header |     block     |
 * |        |               |
 * '------------------------'
 */
typedef union memory_block_header {
    struct {
        size_t size;
        unsigned is_free;
        union memory_block_header *next;
    } s;
    align stub;
} header_t;


header_t *head = NULL;
header_t *tail = NULL;

header_t *get_free_block(size_t size) {
    header_t *curr_ptr = head;
    while(curr_ptr) {
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

    size_t total_size;
    total_size = ALIGN16(size + sizeof(header_t));

    void *block = NULL;
    header_t *header = NULL;

    pthread_mutex_lock(&global_malloc_lock);

    header = get_free_block(size);
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
        head = header;
    }
    if (tail) {
        tail->s.next = header;
    }
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)(header + 1);
}
