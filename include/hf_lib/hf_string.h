//Functions for handling null terminated utf-8 strings using bounds checking by default
//Dependencies:
//- stdbool.h
//- stddef.h

#ifndef HF_STRING_H
#define HF_STRING_H

#include <stdbool.h>
#include <stddef.h>

//Returns true if contents of left are exactly the same as the contents of right.
bool hf_string_equal(const char* left, const char* right);

//Returns the lenght in bytes of a utf-8 null terminated string.
size_t hf_string_length_bytes(const char* string);

//Returns the lenght in unicode codepoints of a utf-8 null terminated string.
size_t hf_string_length_codepoints(const char* string);

//Copies contents of null terminated string into buffer and null terminates the buffer.
//Returns false if string couldn't fit into the buffer, in which case the value in the buffer will be truncated and null terminated.
bool hf_string_copy(const char* string, char* buffer, size_t buffer_size);

//Appends null terminated string to the end of null terminated buffer and null terminates the buffer.
//Returns false if string couldn't fit into the buffer, in which case the value in the buffer will be truncated and null terminated.
bool hf_string_concat(const char* string, char* buffer, size_t buffer_size);

//Given a null terminated string, copies its lowercase version into buffer. Sometimes incorrect, for it not take locales into consideration.
//Returns false if lowercase string could not fit the buffer.
bool hf_string_to_lower(const char* restrict string, char* restrict buffer, size_t buffer_size);

//Given a null terminated string, copies its uppercase version into buffer. Sometimes incorrect, for it not take locales into consideration.
//Returns false if uppercase string could not fit the buffer.
bool hf_string_to_upper(const char* restrict string, char* restrict buffer, size_t buffer_size);


//Attempts to parse string into an unsigned long long. If the value is not found right away, aborts.
//Returns a pointer to the character after the last character of the unsigned long long, or NULL if the provided string does not start as a parsable unsigned long long.
const char* hf_string_parse_ull(const char* string, unsigned long long* value);

//Attempts to parse string into a long long. If the value is not found right away, aborts.
//Returns a pointer to the character after the last character of the long long, or NULL if the provided string does not start as a parsable long long.
const char* hf_string_parse_ll(const char* string, long long* value);

//Attempts to parse string into an unsigned integer. If the value is not found right away, aborts.
//Returns a pointer to the character after the last character of the unsigned integer, or NULL if the provided string does not start as a parsable unsigned integer.
const char* hf_string_parse_uint(const char* string, unsigned int* value);

//Attempts to parse string into an integer. If the value is not found right away, aborts.
//Returns a pointer to the character after the last character of the integer, or NULL if the provided string does not start as a parsable integer.
const char* hf_string_parse_int(const char* string, int* value);

//Attempts to parse string into an double. If the value is not found right away, aborts.
//Returns a pointer to the character after the last character of the double, or NULL if the provided string does not start as a parsable double.
const char* hf_string_parse_double(const char* string, double* value);

//Attempts to parse string into an float. If the value is not found right away, aborts.
//Returns a pointer to the character after the last character of the float, or NULL if the provided string does not start as a parsable float.
const char* hf_string_parse_float(const char* string, float* value);

//Finds and parses the first occurence of an unsigned long long in string, storing its value in the provided pointer.
//Returns a pointer to the character after the last character of the unsigned long long, or NULL if reached the end of the string without finding a parsable unsigned long long.
const char* hf_string_find_ull(const char* string, unsigned long long* value);

//Finds and parses the first occurence of an long long in string, storing its value in the provided pointer.
//Returns a pointer to the character after the last character of the long long, or NULL if reached the end of the string without finding a parsable long long.
const char* hf_string_find_ll(const char* string, long long* value);

//Finds and parses the first occurence of an unsigned integer in string, storing its value in the provided pointer.
//Returns a pointer to the character after the last character of the unsigned integer, or NULL if reached the end of the string without finding a parsable unsigned integer.
const char* hf_string_find_uint(const char* string, unsigned int* value);

//Finds and parses the first occurence of an integer in string, storing its value in the provided pointer.
//Returns a pointer to the character after the last character of the integer, or NULL if reached the end of the string without finding a parsable integer.
const char* hf_string_find_int(const char* string, int* value);

//Finds and parses the first occurence of a double in string, storing its value in the provided pointer.
//Returns a pointer to the character after the last character of the double, or NULL if reached the end of the string without finding a parsable double.
const char* hf_string_find_double(const char* string, double* value);

//Finds and parses the first occurence of a float in string, storing its value in the provided pointer.
//Returns a pointer to the character after the last character of the float, or NULL if reached the end of the string without finding a parsable float.
const char* hf_string_find_float(const char* string, float* value);

#endif//HF_STRING_H
