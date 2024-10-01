#include "../include/hf_string.h"

#define HF_STRING_MASK_UTF8_START ((1 << 7) | (1 << 6))
#define HF_STRING_MASK_UTF8_CONTINUATION (1 << 7)

bool hf_string_equal(const char* left, const char* right) {
    while(*left && *right) {
        if(*left != *right) {
            return false;
        }
        left++;
        right++;
    }
    return *left == *right;
}

size_t hf_string_length_bytes(const char* string) {
    size_t len = 0;
    while(*string) {
        string++;
        len++;
    }
    return len;
}

size_t hf_string_length_codepoints(const char* string) {
    size_t len = 0;
    while(*string) {
        if((*(string++) & HF_STRING_MASK_UTF8_START) == HF_STRING_MASK_UTF8_START) {//
            do {
                string++;
            } while((*string & HF_STRING_MASK_UTF8_START) == HF_STRING_MASK_UTF8_CONTINUATION);
        }

        len++;
    }
    return len;
}

bool hf_string_copy(const char* string, char* buffer, size_t buffer_size) {
    if(buffer_size == 0) {
        return false;
    }

    size_t index = 0;
    while(index < (buffer_size - 1) && *string) {
        buffer[index++] = *string;
        string++;
    }
    bool retval = !(*string);
    while((*string & HF_STRING_MASK_UTF8_START) == HF_STRING_MASK_UTF8_CONTINUATION) {//push index back to start of codepoint if applicable
        string--;
        index--;
    }
    buffer[index] = '\0';
    return retval;
}

bool hf_string_concat(const char* string, char* buffer, size_t buffer_size) {
    size_t new_len = buffer_size;
    while(*buffer && new_len > 0) {
        buffer++;
        new_len--;
    }
    return hf_string_copy(string, buffer, new_len);
}

//will convert a utf8 character to a unicode codepoint and return it
static long internal_hf_string_utf8_to_unicode(const char* string) {
    int set_bits = 0;
    for(int i = 0; i < 4; i++) {
        if(((*string) >> (7 - i)) & 1) {
            set_bits++;
        }
        else {
            break;
        }
    }

    long retval = 0;
    long shift = 0;
    for(int i = (set_bits > 0 ? set_bits - 1 : 0); i >= 0; i--) {
        const char byte = string[i];
        int num_bits = i == 0 ? (7 - set_bits) : 6;
        for(int b = 0; b < num_bits; b++) {
            if((byte >> b) & 1) {
                retval |= (1L << shift);
            }
            shift++;
        }
    }
    return retval;
}

//will set string to the utf8 string corresponding to the provided unicode codepoint
static void internal_hf_string_set_utf8_from_unicode(char* string, long unicode) {
    int required_bits = 0;
    for(int i = 0; i < (int)(sizeof(unsigned long) * 8); i++) {
        if((unicode >> i) & 1) {
            required_bits = i + 1;
        }
    }

    int required_bytes = required_bits <= 7 ? 1 : (required_bits <= 11 ? 2 : (required_bits <= 16 ? 3 : 4));
    unsigned int shift = 0;
    for(int i = required_bytes - 1; i >= 0; i--) {
        char* byte_ptr = string + i;
        *byte_ptr = 0;
        if(i == 0 && required_bytes > 1) {//set initial string bits
            for(int b = 0; b < required_bytes; b++) {
                *byte_ptr |= 1 << (7 - b);
            }
        }
        else if(i != 0) {//set continuation bits
            *byte_ptr |= HF_STRING_MASK_UTF8_CONTINUATION;
        }

        int num_bits = required_bytes == 1 ? 7 : (i == 0 ? 7 - required_bytes : 6);
        for(int b = 0; b < num_bits; b++) {
            *byte_ptr |= (((unicode >> shift) & 1) << b);
            shift++;
        }
    }
}

static long internal_hf_string_unicode_to_upper(
    long unicode_value,
    long* range_table, size_t range_table_len,
    long* alternating_table, size_t alternating_table_len,
    long* triplet_table, size_t triplet_table_len,
    long* direct_table, size_t direct_table_len
) {
    for(size_t i = 0; i < range_table_len; i++) {
        long offset = range_table[i * 3 + 2];
        long start = range_table[i * 3] + offset;
        long end = range_table[i * 3 + 1] + offset;

        if(unicode_value >= start && unicode_value <= end) {
            return unicode_value - offset;
        }
    }

    for(size_t i = 0; i < alternating_table_len; i++) {
        long start = alternating_table[i * 2];
        long end = alternating_table[i * 2 + 1] + 1;

        long dist_to_start = unicode_value - start;
        if(unicode_value >= start && unicode_value <= end) {
            return unicode_value - (dist_to_start % 2);
        }
    }

    for(size_t i = 0; i < triplet_table_len; i++) {
        long start = triplet_table[i * 2];
        long end = triplet_table[i * 2 + 1] + 2;

        long dist_to_start = unicode_value - start;
        if(unicode_value >= start && unicode_value <= end) {
            return unicode_value - (dist_to_start % 3);
        }
    }

    for(size_t i = 0; i < direct_table_len; i++) {
        long key = direct_table[i * 2 + 1];

        if(unicode_value == key) {
            return direct_table[i * 2];
        }
    }

    return unicode_value;
}

static long internal_hf_string_unicode_to_lower(
    long unicode_value,
    long* range_table, size_t range_table_len,
    long* alternating_table, size_t alternating_table_len,
    long* triplet_table, size_t triplet_table_len,
    long* direct_table, size_t direct_table_len
) {
    for(size_t i = 0; i < range_table_len; i++) {
        long offset = range_table[i * 3 + 2];
        long start = range_table[i * 3];
        long end = range_table[i * 3 + 1];

        if(unicode_value >= start && unicode_value <= end) {
            return unicode_value + offset;
        }
    }

    for(size_t i = 0; i < alternating_table_len; i++) {
        long start = alternating_table[i * 2];
        long end = alternating_table[i * 2 + 1] + 1;

        long shift = 1 - ((unicode_value - start) % 2);
        if(unicode_value >= start && unicode_value <= end) {
            return unicode_value + shift;
        }
    }

    for(size_t i = 0; i < triplet_table_len; i++) {
        long start = triplet_table[i * 2];
        long end = triplet_table[i * 2 + 1] + 2;

        long shift = 2 - ((unicode_value - start) % 3);
        if(unicode_value >= start && unicode_value <= end) {
            return unicode_value + shift;
        }
    }

    for(size_t i = 0; i < direct_table_len; i++) {
        long key = direct_table[i * 2];

        if(unicode_value == key) {
            return direct_table[i * 2 + 1];
        }
    }

    return unicode_value;
}

static bool internal_hf_string_to_case(const char* restrict string, char* restrict buffer, size_t buffer_size, bool to_upper) {
    if(buffer_size == 0) {
        return false;
    }

    //auto-generated conversion tables based on https://www.unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
    long range_table[] = {
        65, 90, 32,
        192, 214, 32,
        216, 222, 32,
        393, 394, 205,
        433, 434, 217,
        904, 906, 37,
        910, 911, 63,
        913, 939, 32,
        1021, 1023, -130,
        1024, 1039, 80,
        1040, 1071, 32,
        1329, 1366, 48,
        4256, 4301, 7264,
        5024, 5103, 38864,
        5104, 5109, 8,
        7312, 7359, -3008,
        7944, 7951, -8,
        7960, 7965, -8,
        7976, 7983, -8,
        7992, 7999, -8,
        8008, 8013, -8,
        8040, 8047, -8,
        8072, 8079, -8,
        8088, 8095, -8,
        8104, 8111, -8,
        8120, 8121, -8,
        8122, 8123, -74,
        8136, 8139, -86,
        8152, 8153, -8,
        8154, 8155, -100,
        8168, 8169, -8,
        8170, 8171, -112,
        8184, 8185, -128,
        8186, 8187, -126,
        8544, 8559, 16,
        9398, 9423, 26,
        11264, 11311, 48,
        11390, 11391, -10815,
        65313, 65338, 32,
        66560, 66599, 40,
        66736, 66771, 40,
        66928, 66965, 39,
        68736, 68786, 64,
        71840, 71871, 32,
        93760, 93791, 32,
        125184, 125217, 34,
    };
    const size_t range_table_len = sizeof(range_table) / sizeof(range_table[0]) / 3;

    long alternating_table[] = {
        256, 302,
        306, 310,
        313, 327,
        330, 374,
        377, 381,
        386, 388,
        416, 420,
        435, 437,
        461, 475,
        478, 494,
        504, 542,
        546, 562,
        582, 590,
        880, 882,
        984, 1006,
        1120, 1152,
        1162, 1214,
        1217, 1229,
        1232, 1326,
        7680, 7828,
        7840, 7934,
        11367, 11371,
        11392, 11490,
        11499, 11501,
        42560, 42604,
        42624, 42650,
        42786, 42798,
        42802, 42862,
        42873, 42875,
        42878, 42886,
        42896, 42898,
        42902, 42920,
        42932, 42946,
        42951, 42960,
        42966, 42968,
    };
    const size_t alternating_table_len = sizeof(alternating_table) / sizeof(alternating_table[0]) / 2;

    long triplet_table[] = {
        452, 458,
        497, 497,
    };
    const size_t triplet_table_len = sizeof(triplet_table) / sizeof(triplet_table[0]) / 2;

    long direct_table[] = {
        304, 105,
        376, 255,
        385, 595,
        390, 596,
        391, 392,
        395, 396,
        398, 477,
        399, 601,
        400, 603,
        401, 402,
        403, 608,
        404, 611,
        406, 617,
        407, 616,
        408, 409,
        412, 623,
        413, 626,
        415, 629,
        422, 640,
        423, 424,
        425, 643,
        428, 429,
        430, 648,
        431, 432,
        439, 658,
        440, 441,
        444, 445,
        500, 501,
        502, 405,
        503, 447,
        544, 414,
        570, 11365,
        571, 572,
        573, 410,
        574, 11366,
        577, 578,
        579, 384,
        580, 649,
        581, 652,
        886, 887,
        895, 1011,
        902, 940,
        908, 972,
        975, 983,
        1012, 952,
        1015, 1016,
        1017, 1010,
        1018, 1019,
        1216, 1231,
        7838, 223,
        8025, 8017,
        8027, 8019,
        8029, 8021,
        8031, 8023,
        8124, 8115,
        8140, 8131,
        8172, 8165,
        8188, 8179,
        8486, 969,
        8490, 107,
        8491, 229,
        8498, 8526,
        8579, 8580,
        11360, 11361,
        11362, 619,
        11363, 7549,
        11364, 637,
        11373, 593,
        11374, 625,
        11375, 592,
        11376, 594,
        11378, 11379,
        11381, 11382,
        11506, 11507,
        42877, 7545,
        42891, 42892,
        42893, 613,
        42922, 614,
        42923, 604,
        42924, 609,
        42925, 620,
        42926, 618,
        42928, 670,
        42929, 647,
        42930, 669,
        42931, 43859,
        42948, 42900,
        42949, 642,
        42950, 7566,
        42997, 42998,
    };
    const size_t direct_table_len = sizeof(direct_table) / sizeof(direct_table[0]) / 2;

    size_t buffer_index = 0;
    bool retval = true;

    const char* itr = string;
    while(*itr) {
        if(buffer_index >= (buffer_size - 1)) {
            retval = false;
            break;
        }

        long unicode = internal_hf_string_utf8_to_unicode(itr);
        long(*conversion_func)(long, long*, size_t, long*, size_t, long*, size_t, long*, size_t) = to_upper ?
            internal_hf_string_unicode_to_upper :
            internal_hf_string_unicode_to_lower
        ;
        long new_unicode = conversion_func(
            unicode,
            range_table, range_table_len,
            alternating_table, alternating_table_len,
            triplet_table, triplet_table_len,
            direct_table, direct_table_len
        );

        const char* to_copy;
        char new_buffer[5] = { '\0' };
        if(unicode != new_unicode) {
            internal_hf_string_set_utf8_from_unicode(new_buffer, new_unicode);
            to_copy = new_buffer;
        }
        else {
            to_copy = itr;
        }

        do {//copy character into buffer
            if(buffer_index >= (buffer_size - 1)) {//could not fit buffer, erase previous codepoint
                retval = false;
                while((buffer[buffer_index] & HF_STRING_MASK_UTF8_START) != HF_STRING_MASK_UTF8_START && buffer_index > 0) {
                    buffer_index--;
                }
                goto out;
            }
            buffer[buffer_index++] = *to_copy;
            to_copy++;
        } while((*to_copy & HF_STRING_MASK_UTF8_START) == HF_STRING_MASK_UTF8_CONTINUATION);

        //step itr into next character
        do { itr++; } while((*itr & HF_STRING_MASK_UTF8_START) == HF_STRING_MASK_UTF8_CONTINUATION);
    }
    out:

    buffer[buffer_index] = '\0';
    return retval;
}

bool hf_string_to_lower(const char* restrict string, char* restrict buffer, size_t buffer_size) {
    return internal_hf_string_to_case(string, buffer, buffer_size, false);
}

bool hf_string_to_upper(const char* restrict string, char* restrict buffer, size_t buffer_size) {
    return internal_hf_string_to_case(string, buffer, buffer_size, true);
}


const char* hf_string_parse_ull(const char* string, unsigned long long* value) {
    if(!string || string[0] < '0' || string[0] > '9') {
        return NULL;
    }

    const char* ptr = string;
    const char* start = ptr;//due to previous checks, start is guaranteed to be a number here
    do {
        ptr++;
    } while(ptr[0] >= '0' && ptr[0] <= '9');
    const char* end = ptr;

    if(value) {
        unsigned long long mul = 1;
        *value = 0;
        while(ptr != start) {
            ptr--;
            *value += mul * (unsigned long long)(*ptr - '0');
            mul *= 10;
        }
    }

    return end;
}

const char* hf_string_parse_ll(const char* string, long long* value) {
    if(!string) {
        return NULL;
    }

    bool neg = string[0] == '-';
    unsigned long long ull;

    const char* ptr = hf_string_parse_ull(neg ? &string[1] : string, &ull);
    if(value && ptr) {
        *value = (long long)ull;
        if(neg) {
            *value = -*value;
        }
    }
    return ptr;
}

const char* hf_string_parse_uint(const char* string, unsigned int* value) {
    unsigned long long ull;
    const char* ptr = hf_string_parse_ull(string, &ull);
    if(ptr && value) {
        *value = (unsigned int)ull;
    }
    return ptr;
}

const char* hf_string_parse_int(const char* string, int* value) {
    long long ll;
    const char* ptr = hf_string_parse_ll(string, &ll);
    if(ptr && value) {
        *value = (int)ll;
    }
    return ptr;
}

const char* hf_string_parse_double(const char* string, double* value) {
    if(!string) {
        return NULL;
    }

    bool neg = string[0] == '-';

    long long integer = 0;
    unsigned long long fraction = 0;
    unsigned long long fraction_digits = 0;
    long long exponent = 0;

    const char* ptr = hf_string_parse_ll(string, &integer);
    if(!ptr) {
        return NULL;
    }

    if(ptr[0] == '.') {
        const char* tmp = hf_string_parse_ull(&ptr[1], &fraction);
        if(tmp) {
            ptr++;
            while(ptr != tmp) {
                ptr++;
                fraction_digits++;
            }
        }
    }
    if(ptr[0] == 'e') {
        const char* tmp = hf_string_parse_ll(&ptr[1], &exponent);
        if(tmp) {
            ptr = tmp;
        }
    }

    if(value) {
        double dinteger = (double)integer;

        double dfraction = neg ? -(double)fraction : (double)fraction;
        while(fraction_digits) {
            fraction_digits--;
            dfraction /= 10.0;
        }
        *value = dinteger + dfraction;

        while(exponent > 0) {
            *value *= 10.0;
            exponent--;
        }
        while(exponent < 0) {
            *value /= 10.0;
            exponent++;
        }
    }

    return ptr;
}

const char* hf_string_parse_float(const char* string, float* value) {
    double d;
    const char* ptr = hf_string_parse_double(string, &d);
    if(ptr && value) {
        *value = (float)d;
    }
    return ptr;
}

const char* hf_string_find_ull(const char* string, unsigned long long* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hf_string_parse_ull(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

const char* hf_string_find_ll(const char* string, long long* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hf_string_parse_ll(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

const char* hf_string_find_uint(const char* string, unsigned int* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hf_string_parse_uint(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

const char* hf_string_find_int(const char* string, int* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hf_string_parse_int(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

const char* hf_string_find_double(const char* string, double* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hf_string_parse_double(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

const char* hf_string_find_float(const char* string, float* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hf_string_parse_float(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}
