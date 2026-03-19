#include "microheap.h"
#include <stdio.h>
#include <string.h>

void print_separator(const char *title) {
    printf("\n");
    printf("=================================================\n");
    printf("  %s\n", title);
    printf("=================================================\n");
}

void test_malloc() {
    print_separator("TEST 1: my_malloc()");
    
    // Allocate memory for an array of integers
    int *arr = (int *)my_malloc(sizeof(int) * 10);
    if (arr == NULL) {
        printf("❌ Memory allocation failed!\n");
        return;
    }
    printf("✅ Successfully allocated memory for 10 integers\n");
    
    // Initialize and print values
    printf("Writing values: ");
    for (int i = 0; i < 10; i++) {
        arr[i] = (i + 1) * 10;
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    my_free(arr);
}

void test_calloc() {
    print_separator("TEST 2: my_calloc()");
    
    // Allocate zeroed memory for 20 integers
    int *zeros = (int *)my_calloc(20, sizeof(int));
    if (zeros == NULL) {
        printf("❌ Memory allocation failed!\n");
        return;
    }
    printf("✅ Successfully allocated zeroed memory for 20 integers\n");
    
    // Verify all values are zero
    printf("Verifying zero-initialization: ");
    int all_zeros = 1;
    for (int i = 0; i < 20; i++) {
        if (zeros[i] != 0) {
            all_zeros = 0;
            break;
        }
    }
    printf("%s\n", all_zeros ? "✅ All values are zero" : "❌ Some values are non-zero");
    
    // Use the memory
    for (int i = 0; i < 20; i++) {
        zeros[i] = i * 2;
    }
    printf("Updated first 10 values: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", zeros[i]);
    }
    printf("\n");
    
    my_free(zeros);
}

void test_realloc() {
    print_separator("TEST 3: my_realloc()");
    
    // Allocate small block
    int *data = (int *)my_malloc(sizeof(int) * 5);
    if (data == NULL) {
        printf("❌ Initial allocation failed!\n");
        return;
    }
    printf("✅ Allocated memory for 5 integers\n");
    
    // Fill with data
    for (int i = 0; i < 5; i++) {
        data[i] = i + 100;
    }
    printf("Original values: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    // Reallocate to larger size
    int *new_data = (int *)my_realloc(data, sizeof(int) * 15);
    if (new_data == NULL) {
        printf("❌ Reallocation failed!\n");
        my_free(data);
        return;
    }
    printf("✅ Reallocated to 15 integers\n");
    
    // Verify old data is preserved
    printf("Data after realloc (first 5): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", new_data[i]);
    }
    printf("\n");
    
    // Fill new space
    for (int i = 5; i < 15; i++) {
        new_data[i] = i + 100;
    }
    printf("All 15 values: ");
    for (int i = 0; i < 15; i++) {
        printf("%d ", new_data[i]);
    }
    printf("\n");
    
    my_free(new_data);
}

void test_string_allocation() {
    print_separator("TEST 4: String Operations");
    
    // Allocate memory for a string
    char *message = (char *)my_malloc(50);
    if (message == NULL) {
        printf("❌ String allocation failed!\n");
        return;
    }
    printf("✅ Allocated 50 bytes for string\n");
    
    // Copy string
    strcpy(message, "Hello from MicroHeap allocator!");
    printf("Stored message: \"%s\"\n", message);
    
    // Reallocate for longer string
    char *longer = (char *)my_realloc(message, 100);
    if (longer == NULL) {
        printf("❌ String reallocation failed!\n");
        my_free(message);
        return;
    }
    printf("✅ Reallocated to 100 bytes\n");
    
    strcat(longer, " This is amazing!");
    printf("Extended message: \"%s\"\n", longer);
    
    my_free(longer);
}

void test_multiple_allocations() {
    print_separator("TEST 5: Multiple Allocations");
    
    printf("Allocating multiple blocks of different sizes...\n");
    
    void *blocks[5];
    size_t sizes[] = {16, 32, 64, 128, 256};
    
    for (int i = 0; i < 5; i++) {
        blocks[i] = my_malloc(sizes[i]);
        if (blocks[i] == NULL) {
            printf("❌ Allocation %d failed!\n", i + 1);
        } else {
            printf("✅ Block %d: %zu bytes allocated at %p\n", 
                   i + 1, sizes[i], blocks[i]);
        }
    }
    
    // Free in reverse order
    printf("\nFreeing blocks in reverse order...\n");
    for (int i = 4; i >= 0; i--) {
        if (blocks[i] != NULL) {
            my_free(blocks[i]);
            printf("✅ Block %d freed\n", i + 1);
        }
    }
}

void test_struct_allocation() {
    print_separator("TEST 6: Structure Allocation");
    
    // Define a sample structure
    typedef struct {
        int id;
        char name[32];
        double value;
    } Record;
    
    // Allocate array of structures using calloc
    Record *records = (Record *)my_calloc(3, sizeof(Record));
    if (records == NULL) {
        printf("❌ Structure allocation failed!\n");
        return;
    }
    printf("✅ Allocated memory for 3 Record structures\n");
    
    // Initialize structures
    records[0].id = 1;
    strcpy(records[0].name, "Alice");
    records[0].value = 99.5;
    
    records[1].id = 2;
    strcpy(records[1].name, "Bob");
    records[1].value = 87.3;
    
    records[2].id = 3;
    strcpy(records[2].name, "Charlie");
    records[2].value = 95.8;
    
    // Display structures
    printf("\nRecords stored:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Record %d: ID=%d, Name=%s, Value=%.1f\n",
               i + 1, records[i].id, records[i].name, records[i].value);
    }
    
    my_free(records);
}

int main(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║     MICROHEAP MEMORY ALLOCATOR DEMO          ║\n");
    printf("║   Custom malloc/free/calloc/realloc          ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    test_malloc();
    test_calloc();
    test_realloc();
    test_string_allocation();
    test_multiple_allocations();
    test_struct_allocation();
    
    print_separator("ALL TESTS COMPLETED");
    printf("✅ MicroHeap allocator demonstration finished!\n\n");
    
    return 0;
}
