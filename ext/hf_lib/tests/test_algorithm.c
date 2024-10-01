#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "../include/hf_algorithm.h"

#define assert_array(arr, size, ...)\
do {\
    int assertion_arr[] = { __VA_ARGS__ };\
    for(size_t i = 0; i < size; i++) {\
        if(arr[i] != assertion_arr[i]) {\
            return EXIT_FAILURE;\
        }\
    }\
} while(0)

void print_arr_int(const char* title, int* arr, size_t arr_size) {
    printf("%s: { ", title);
    for(size_t i = 0; i < arr_size; i++) {
        if(i != 0) {
            printf(", ");
        }
        printf("%d", arr[i]);
    }
    printf(" }\n");
}

void print_arr_char(const char* title, char* arr, size_t arr_size) {
    printf("%s: { ", title);
    for(size_t i = 0; i < arr_size; i++) {
        if(i != 0) {
            printf(", ");
        }
        printf("%c", arr[i]);
    }
    printf(" }\n");
}

void query_select_char(void* src, void* dest) {
    *(char*)dest = (char)*(int*)src + 65;
}

int compare(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

static void reset_array(int* arr) {
    arr[0] = 5;
    arr[1] = 3;
    arr[2] = 4;
    arr[3] = 1;
    arr[4] = 2;
}

int main(int argc, char** argv) {
    int arr[5];
    reset_array(arr);
    print_arr_int("original array", arr, sizeof(arr) / sizeof(arr[0]));

    hf_sort_bubble(arr, 5, sizeof(int), compare);
    print_arr_int("bubble sorted array", arr, 5);
    assert_array(arr, 5, 1, 2, 3, 4, 5);

    reset_array(arr);
    hf_sort_selection(arr, 5, sizeof(int), compare);
    print_arr_int("selection sorted array", arr, 5);
    assert_array(arr, 5, 1, 2, 3, 4, 5);

    char select_char[5];
    hf_query_select(arr, select_char, sizeof(int), sizeof(char), 5, query_select_char);
    print_arr_char("select", select_char, 5);
    assert_array(select_char, 5, 'B', 'C', 'D', 'E', 'F');

    return EXIT_SUCCESS;
}