#ifndef HF_ALGORITHM_H
#define HF_ALGORITHM_H

#include <stdbool.h>
#include <stddef.h>

void hf_sort_bubble(void* items, size_t item_count, size_t item_size, int(*compare)(void* a, void* b));
void hf_sort_selection(void* items, size_t item_count, size_t item_size, int(*compare)(void* a, void* b));

void hf_sort_with_context_bubble(void* items, size_t item_count, size_t item_size, int(*compare)(void* a, void* b, void* context), void* context);
void hf_sort_with_context_selection(void* items, size_t item_count, size_t item_size, int(*compare)(void* a, void* b, void* context), void* context);

void hf_query_select(void* src, void* dest, size_t src_item_size, size_t dest_item_size, size_t item_count, void(*expression)(void* src, void* dest));
void hf_query_where(void* src, void* dest, size_t item_size, size_t item_count, size_t* new_item_count, bool(*predicate)(void* value));

#endif//HF_ALGORITHM_H
