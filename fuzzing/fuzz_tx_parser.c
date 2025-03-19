#include <stdio.h>
#include <string.h>

#include "../src/utils.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    char seed[55] = {0};

    int result = encode_base26(data, size, seed, 55);

    if (result == 0 && strcmp(seed, "eiszcqa") == 0) {
        printf("%s\n", seed);
        return 0;
    }

    return 0;
}
