//Functions for handling file paths
//Dependencies:
//- stdbool.h
//- stddef.h
//- hf_string.h

#ifndef HF_PATH_H
#define HF_PATH_H

#define HF_PATH_MAX_PATH_LENGTH 512

#include <stdbool.h>
#include <stddef.h>

//returns true if the path is valid according to the system rules for path naming
bool hf_path_valid(const char* path);
//given two valid paths, returns true if the paths resolve to the same when normalized, or false otherwise.
bool hf_path_equal(const char* left_path, const char* right_path);

//normalizes a path, removing redundancies or extra separators, saving the results into the provided buffer
//returns true on success or false if normalized value could not fit buffer
bool hf_path_normalize(const char* path, char* buffer, size_t buffer_size);
//gets the parent of the provided path, saving it into the provided buffer
//returns true on success or false if value could not fit into the buffer
bool hf_path_parent(const char* path, char* buffer, size_t buffer_size);
//gets the extension of the file pointed by path, if applicable, and stores it into the provided buffer
//returns true on success or false if extension string could not fit into the buffer.
bool hf_path_extension(const char* path, char* buffer, size_t buffer_size);

#endif//HF_PATH_H
