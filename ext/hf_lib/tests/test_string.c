#include <stdlib.h>
#include <assert.h>

#include "../include/hf_string.h"

int main(int argc, char** argv) {
    assert(hf_string_equal("ascii", "ascii"));
    assert(hf_string_equal("caça", "caça"));
    assert(hf_string_equal("바나나", "바나나"));

    assert(hf_string_length_bytes("ççç") == 6);
    assert(hf_string_length_codepoints("ççç") == 3);

    assert(hf_string_length_bytes("바나나") == 9);
    assert(hf_string_length_codepoints("바나나") == 3);

    {
        char buffer[5];
        assert(!hf_string_copy("caça", buffer, 5));
        assert(hf_string_length_bytes(buffer) == 4);

        assert(!hf_string_copy("바나나", buffer, 5));
        assert(hf_string_length_bytes(buffer) == 3);
    }
    {
        char buffer[10];
        assert(hf_string_copy("바", buffer, 10));
        assert(hf_string_concat("ço", buffer, 10));
        assert(hf_string_equal(buffer, "바ço"));
        assert(hf_string_length_bytes(buffer) == 6);
        assert(hf_string_length_codepoints(buffer) == 3);

        assert(!hf_string_concat("바ço", buffer, 10));
        assert(hf_string_equal(buffer, "바ço바"));
        assert(hf_string_length_bytes(buffer) == 9);
        assert(hf_string_length_codepoints(buffer) == 4);
    }
    {
        char buffer[10];
        assert(hf_string_to_upper("abcde", buffer, 10));
        assert(hf_string_equal(buffer, "ABCDE"));

        assert(hf_string_to_lower("ABCDE", buffer, 10));
        assert(hf_string_equal(buffer, "abcde"));


        assert(!hf_string_to_upper("àéa", buffer, 5));
        assert(hf_string_equal(buffer, "ÀÉ"));

        assert(hf_string_to_upper("àéa", buffer, 10));
        assert(hf_string_equal(buffer, "ÀÉA"));

        assert(!hf_string_to_lower("ÀÉA", buffer, 5));
        assert(hf_string_equal(buffer, "àé"));

        assert(hf_string_to_lower("ÀÉA", buffer, 10));
        assert(hf_string_equal(buffer, "àéa"));


        assert(hf_string_to_upper("øþ", buffer, 10));
        assert(hf_string_equal(buffer, "ØÞ"));

        assert(hf_string_to_lower("ØÞ", buffer, 10));
        assert(hf_string_equal(buffer, "øþ"));

        assert(hf_string_to_upper("żonaŀ", buffer, 10));
        assert(hf_string_equal(buffer, "ŻONAĿ"));

        assert(hf_string_to_lower("ŻONAĿ", buffer, 10));
        assert(hf_string_equal(buffer, "żonaŀ"));

        assert(hf_string_to_upper("ǅǆ", buffer, 10));
        assert(hf_string_equal(buffer, "ǄǄ"));

        assert(hf_string_to_lower("ǅǄ", buffer, 10));
        assert(hf_string_equal(buffer, "ǆǆ"));
    }

    return EXIT_SUCCESS;
}