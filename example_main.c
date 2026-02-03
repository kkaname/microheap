#include "microheap.h"

// this example C file is to demonstrate the 
// heap memory allocator that is built

int main(void) {
    int *ptr = my_malloc(sizeof(int) * 10);
    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        ptr[i] = i + 1;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    return 0;
}
