#include "../include/hf_memory.h"

void hf_memory_arena_init(hf_memory_arena* arena, void* memory, size_t size) {
    *arena = (hf_memory_arena) {
        .size = size,
        .available_size = size,
        .memory_start = memory,
    };
}

void hf_memory_arena_deinit(hf_memory_arena* arena) {
    *arena = (hf_memory_arena){
        .size = 0,
        .available_size = 0,
        .memory_start = NULL,
    };
}

void* hf_memory_arena_get(hf_memory_arena* arena, size_t size) {
    if(arena->available_size < size) {
        return NULL;
    }
    void* mem = ((char*)arena->memory_start) + (arena->size - arena->available_size);
    arena->available_size -= size;
    return mem;
}

void hf_memory_arena_reset(hf_memory_arena* arena) {
    arena->available_size = arena->size;
}
