# MicroHeap - A Lightweight Memory Allocator

<div align="center">

**A custom thread-safe memory allocator implementation in C**

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

</div>

## 📖 Overview

MicroHeap is a custom implementation of dynamic memory allocation functions (`malloc`, `free`, `calloc`, `realloc`) built from scratch using low-level system calls. This project demonstrates fundamental concepts of memory management including heap manipulation, memory alignment, and thread safety.

## ✨ Features

- **Custom Memory Allocation**: Drop-in replacements for standard C memory functions
- **Thread-Safe**: Uses pthread mutexes to ensure safe concurrent access
- **Memory Alignment**: Implements 32-byte alignment for optimal performance
- **Smart Memory Management**: Returns memory to OS when possible, reuses freed blocks
- **Zero Initialization**: Supports `calloc` with proper zero-initialization
- **Memory Reallocation**: Efficient `realloc` with data preservation
- **Overflow Protection**: Built-in overflow checks in `calloc`

## 🛠️ Implementation Details

### Architecture

```
Memory Block Structure:
┌────────────────────────────────────────┐
│  Header (32 bytes aligned)             │
│  ┌──────────────────────────────────┐  │
│  │ size: Size of the block          │  │
│  │ is_free: Free/allocated flag     │  │
│  │ next: Pointer to next block      │  │
│  └──────────────────────────────────┘  │
├────────────────────────────────────────┤
│  User Data Area                        │
│  (requested memory)                    │
│                                        │
└────────────────────────────────────────┘
```

### Key Components

1. **Block Header**: Metadata structure hidden from users
   - Size tracking
   - Free/allocated status
   - Linked list for block management

2. **Memory Alignment**: 16-byte alignment using `ALIGN16` macro

3. **Thread Safety**: Global mutex lock for all operations

4. **Free List Management**: Maintains linked list of all blocks

## 📋 API Reference

### `void *my_malloc(size_t size)`
Allocates a block of memory.
- **Parameters**: `size` - Number of bytes to allocate
- **Returns**: Pointer to allocated memory, or `NULL` on failure

### `void my_free(void *block)`
Frees a previously allocated block.
- **Parameters**: `block` - Pointer to memory to free
- **Behavior**: Returns memory to OS if at end of heap, otherwise marks as free

### `void *my_calloc(size_t num, size_t nsize)`
Allocates and zero-initializes memory for an array.
- **Parameters**: `num` - Number of elements, `nsize` - Size per element
- **Returns**: Pointer to zeroed memory, or `NULL` on failure

### `void *my_realloc(void *block, size_t size)`
Resizes an existing memory block.
- **Parameters**: `block` - Existing block, `size` - New size
- **Returns**: Pointer to resized block, or `NULL` on failure

## 🚀 Getting Started

### Prerequisites

- GCC or Clang compiler
- POSIX-compliant system (Linux, macOS, Unix)
- pthread library

### Building

```bash
# Compile the library with your program
gcc -o example example_program.c microheap.c -lpthread

# Run the example
./example
```

### Usage Example

```c
#include "microheap.h"

int main() {
    // Allocate memory
    int *arr = my_malloc(sizeof(int) * 10);
    
    // Use the memory
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }
    
    // Free the memory
    my_free(arr);
    
    return 0;
}
```

## 📊 Example Output

Run `example_program.c` to see comprehensive demonstrations:

```
╔═══════════════════════════════════════════════╗
║     MICROHEAP MEMORY ALLOCATOR DEMO           ║
║   Custom malloc/free/calloc/realloc           ║
╚═══════════════════════════════════════════════╝

=================================================
  TEST 1: my_malloc()
=================================================
✅ Successfully allocated memory for 10 integers
Writing values: 10 20 30 40 50 60 70 80 90 100
...
```

## 🔍 How It Works

1. **Allocation Process**:
   - Searches for free block in existing list
   - If found, marks it as allocated and returns
   - Otherwise, requests memory from OS using `sbrk()`
   - Adds new block to linked list

2. **Deallocation Process**:
   - Checks if block is at end of heap
   - If yes, returns memory to OS via `sbrk()`
   - Otherwise, marks block as free for reuse

3. **Reallocation Process**:
   - If new size fits in current block, returns same pointer
   - Otherwise, allocates new block, copies data, frees old block

## 🐛 Known Limitations

1. **No Block Splitting**: Large free blocks aren't split for smaller requests
2. **No Coalescing**: Adjacent free blocks aren't merged (fragmentation)
3. **Debug Output**: Contains printf statements that should be removed for production
4. **Limited Error Handling**: Basic error checking, could be more robust
5. **Performance**: Not optimized for high-performance scenarios

## 🎯 Future Improvements

- [ ] Implement block splitting for better memory utilization
- [ ] Add block coalescing to reduce fragmentation
- [ ] Remove debug printf statements
- [ ] Add memory usage statistics and diagnostics
- [ ] Implement best-fit or first-fit strategies
- [ ] Add boundary checking for buffer overflow detection
- [ ] Optimize alignment and reduce overhead
- [ ] Add memory pool support for fixed-size allocations

## 📚 Learning Resources

This project demonstrates:
- Dynamic memory management
- System calls (`sbrk`, `brk`)
- Data structure design (linked lists)
- Thread synchronization
- Memory alignment
- Pointer arithmetic

## 🤝 Contributing

Feel free to fork, improve, and submit pull requests. This is an educational project!

## 📝 License

MIT License - Feel free to use this code for learning and experimentation.

## 👤 Author

Built from scratch as a learning project to understand low-level memory management.

---

<div align="center">

**Made with ❤️ and lots of pointer arithmetic**

</div>
