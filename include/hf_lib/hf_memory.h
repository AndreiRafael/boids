#ifndef HF_MEMORY_H
#define HF_MEMORY_H

#include <stddef.h>

typedef struct HF_MemoryArena_s {
    size_t size;
    size_t available_size;
    void* memory_start;
} hf_memory_arena;

void hf_memory_arena_init(hf_memory_arena* arena, void* memory, size_t size);
void hf_memory_arena_deinit(hf_memory_arena* arena);

void* hf_memory_arena_get(hf_memory_arena* arena, size_t size);
void hf_memory_arena_reset(hf_memory_arena* arena);

#endif//HF_MEMORY_H
