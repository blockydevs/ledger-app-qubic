#include <string.h>
#include "utils.h"
#include <stdbool.h>

static const char BASE26_ALPHABET[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                       'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                       's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

#define ALPHABET_LENGTH     26
#define PADDING_CHAR        'z'
#define WORKING_BUFFER_SIZE 32
#define MAX_RESULT_LENGTH   55

int pad_result_with_z(char *target_string, const size_t target_length) {
    const size_t current_length = strnlen(target_string, MAX_RESULT_LENGTH);

    // If the current length is less than the target, pad with 'z' at the beginning
    if (current_length < target_length) {
        const size_t padding_length = target_length - current_length;
        memmove(target_string + padding_length, target_string, current_length + 1);
        memset(target_string, PADDING_CHAR, padding_length);
    }
    return 0;
}

/**
 * Encodes input hex string into base26 string representation
 * Output buffer does not contain null terminator
 */
int encode_base26(const uint8_t *hex_input,
                  const size_t input_length,
                  char *output,
                  const size_t output_size) {
    if (output_size > MAX_RESULT_LENGTH) {
        // Exceeded max output length
        return 1;
    }

    if (input_length > WORKING_BUFFER_SIZE) {
        return 1;
    }

    if (input_length < 1) {
        return 1;
    }

    uint8_t temporary_working_buffer[WORKING_BUFFER_SIZE];
    char result[MAX_RESULT_LENGTH];
    explicit_bzero(result, sizeof(result));

    memcpy(temporary_working_buffer, hex_input, input_length);

    size_t result_index = MAX_RESULT_LENGTH - 1;
    size_t start_index = MAX_RESULT_LENGTH;

    bool is_zero = false;
    do {
        uint32_t remainder = 0;

        for (size_t i = 0; i < input_length; ++i) {
            const uint32_t dividend = (remainder << 8) | temporary_working_buffer[i];
            temporary_working_buffer[i] = dividend / ALPHABET_LENGTH;
            remainder = dividend % ALPHABET_LENGTH;
        }

        result[result_index--] = BASE26_ALPHABET[remainder];

        if (start_index == MAX_RESULT_LENGTH) {
            start_index = result_index + 1;
        }

        is_zero = true;
        for (size_t i = 0; i < input_length; ++i) {
            if (temporary_working_buffer[i] != 0) {
                is_zero = false;
                break;
            }
        }
    } while (!is_zero);

    const size_t converted_length = MAX_RESULT_LENGTH - (result_index + 1);

    if (converted_length > output_size) {
        return 0x7;
    }

    strncpy(output, &result[result_index + 1], converted_length);

    return 0;
}
