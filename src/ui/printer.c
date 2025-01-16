#include <limits.h>

#include "../globals.h"
#include "printer.h"
#include "util.h"

int print_u64(uint64_t u64, char *out, size_t out_length) {
    BAIL_IF(out_length > INT_MAX);
    uint64_t dVal = u64;
    int outlen = (int) out_length;
    int i = 0;
    int j = 0;

    if (i < (outlen - 1)) {
        do {
            if (dVal > 0) {
                out[i] = (dVal % 10) + '0';
                dVal /= 10;
            } else {
                out[i] = '0';
            }
            i++;
        } while (dVal > 0 && i < outlen);
    }

    BAIL_IF(i >= outlen);

    out[i--] = '\0';

    for (; j < i; j++, i--) {
        int tmp = out[j];
        out[j] = out[i];
        out[i] = tmp;
    }

    return 0;
}