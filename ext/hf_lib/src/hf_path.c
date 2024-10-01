#include "../include/hf_path.h"
#include "../include/hf_string.h"

#define HF_PATH_MAX_PATH_LENGHT_PLUS_ONE (HF_PATH_MAX_PATH_LENGTH + 1)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
    #define HF_PATH_PREFERRED_SEPARATOR ('\\')
    #define HF_PATH_WIN32
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    #define HF_PATH_PREFERRED_SEPARATOR ('/')
    #define HF_PATH_UNIX
#else
    #error "System not supported"
#endif

static bool internal_hf_path_is_separator(const char c) {
    return c == '/' || c == '\\';
}

// TODO: return error code instead of true/false !!!
static bool internal_hf_path_read_token(char* buffer, size_t buffer_size, const char** path_itr) {
    size_t index = 0;
    const char* itr = *path_itr;
    while(*itr) {
        if(internal_hf_path_is_separator(*itr)) {
            if(index > 0) {
                break;
            }
        }
        else {
            if(index >= (buffer_size - 1)) {
                return false;
            }
            buffer[index++] = *itr;
        }
        itr++;
    }

    if(index < buffer_size) {
        buffer[index] = '\0';
    }
    else {
        return false;// TODO: this could falsely indicate end of path??
    }

    *path_itr = itr;// TODO: this could falsely indicate end of path??
    return index > 0;
}

//provided path must be normalized!!
static const char* internal_hf_path_last_token(const char* path) {
    const char* itr = path;
    while(*itr) {//get to end of string
        itr++;
    }

    //go back until a separator or start of path
    while(itr != path) {
        if(internal_hf_path_is_separator(*itr)) {
            break;
        }
        itr--;
    }
    return itr;
}

//provided path must be normalized!!
static bool internal_hf_path_delete_token(char* path) {
    char* itr = path;
    while(*itr) {//get to end of string
        itr++;
    }

    //go back until a separator or start of path
    while(itr != path && !internal_hf_path_is_separator(*itr)) {
        itr--;
    }

    *itr = '\0';
    return itr != path;
}

bool hf_path_valid(const char* path) {
#if defined(HF_PATH_WIN32)
    const char reserved_characters[] = { '<', '>', ':', '\"', '/', '\\', '|', '?', '*' };
    const char* reserved_names[] = {
        "con",
        "prn",
        "aux",
        "nul",
        "com0", "com1", "com2", "com3", "com4", "com5", "com6", "com7", "com8", "com9",
        "lpt0", "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9",
    };
    (void)reserved_names;// TODO: check forbidden names
    char token_buffer[HF_PATH_MAX_PATH_LENGHT_PLUS_ONE];
    const char* itr = path;
    while(internal_hf_path_read_token(token_buffer, HF_PATH_MAX_PATH_LENGHT_PLUS_ONE, &itr)) {
        for(size_t i = 0; i < sizeof(reserved_characters) / sizeof(char); i++) {
            const char char_to_check = reserved_characters[i];

            char* buffer_itr = token_buffer;
            while(*buffer_itr) {
                if(*buffer_itr == char_to_check || char_to_check <= (char)31) {
                    return false;
                }
            }
        }
    }
#elif defined(HF_PATH_UNIX)
    return true;// TODO:
#else
    #error "Unsupported system"
#endif
    return true;
}

bool hf_path_equal(const char* left_path, const char* right_path) {
    char buffer_left[HF_PATH_MAX_PATH_LENGHT_PLUS_ONE];
    char buffer_right[HF_PATH_MAX_PATH_LENGHT_PLUS_ONE];

    return
        hf_path_normalize(left_path, buffer_left, HF_PATH_MAX_PATH_LENGHT_PLUS_ONE) &&
        hf_path_normalize(right_path, buffer_right, HF_PATH_MAX_PATH_LENGHT_PLUS_ONE) &&
        hf_string_equal(buffer_left, buffer_right);
    ;
}

typedef enum ETokenType_e {
    ETOKENTYPE_THIS,
    ETOKENTYPE_PARENT,
    ETOKENTYPE_ROOT,
    ETOKENTYPE_OTHER,
} ETokenType;

static ETokenType internal_hf_path_token_type(const char* token) {
    if(hf_string_equal(token, "..")) {
        return ETOKENTYPE_PARENT;
    }
    if(hf_string_equal(token, ".")) {
        return ETOKENTYPE_THIS;
    }
    return ETOKENTYPE_OTHER;
}

bool hf_path_normalize(const char* path, char* buffer, size_t buffer_size) {
    if(buffer_size == 0) {
        return false;
    }
    buffer[0] = '\0';

    char separator_str[2];
    separator_str[0] = HF_PATH_PREFERRED_SEPARATOR;
    separator_str[1] = '\0';

    int depth = 0;

    char token_buffer[HF_PATH_MAX_PATH_LENGHT_PLUS_ONE];
    while(internal_hf_path_read_token(token_buffer, HF_PATH_MAX_PATH_LENGHT_PLUS_ONE, &path)) {
        ETokenType type = internal_hf_path_token_type(token_buffer);

        if(type == ETOKENTYPE_PARENT && depth > 0) {
            internal_hf_path_delete_token(buffer);
            depth--;
        }
        else if(type != ETOKENTYPE_THIS || buffer[0] == '\0') {
            if(type == ETOKENTYPE_PARENT && internal_hf_path_token_type(buffer) == ETOKENTYPE_THIS) {// . became irrelevant
                buffer[0] = '\0';
            }

            if(buffer[0] != '\0') {//put separator before new token
                if(!hf_string_concat(separator_str, buffer, buffer_size)) {
                    return false;
                }
            }
            if(!hf_string_concat(token_buffer, buffer, buffer_size)) {
                return false;
            }

            if(type == ETOKENTYPE_PARENT) {
                depth--;
            }
            else if(type != ETOKENTYPE_THIS) {
                depth++;
            }
        }
    }

    return !(*path);
}

bool hf_path_parent(const char* path, char* buffer, size_t buffer_size) {
    char normalized_buffer[HF_PATH_MAX_PATH_LENGHT_PLUS_ONE];
    if(hf_path_normalize(path, normalized_buffer, HF_PATH_MAX_PATH_LENGHT_PLUS_ONE)) {
        const char* last_token = internal_hf_path_last_token(path);
        if(hf_string_equal(last_token, ".")) {
            return hf_string_copy("..", buffer, buffer_size);
        }
        if(hf_string_equal(last_token, "..")) {
            char back_str[4];
            back_str[0] = HF_PATH_PREFERRED_SEPARATOR;
            back_str[1] = '.';
            back_str[2] = '.';
            back_str[3] = '\0';

            return
                hf_string_copy(normalized_buffer, buffer, buffer_size) &&
                hf_string_concat(back_str, buffer, buffer_size);
            ;
        }

        return
            internal_hf_path_delete_token(normalized_buffer) &&
            hf_string_copy(normalized_buffer, buffer, buffer_size)
        ;
    }

    return false;
}

bool hf_path_extension(const char* path, char* buffer, size_t buffer_size) {
    if(buffer_size == 0) {
        return false;
    }

    const char* last_token = internal_hf_path_last_token(path);
    if(hf_string_equal(last_token, ".") || hf_string_equal(last_token, "..")) {
        buffer[0] = '\0';
        return true;
    }

    const char* itr = last_token;
    const char* copy_position = itr;
    while(*itr) {
        if(*itr == '.') {
            copy_position = itr;
        }
        itr++;
    }

    if(copy_position != last_token) {
        return hf_string_copy(copy_position, buffer, buffer_size);
    }
    buffer[0] = '\0';
    return true;
}
