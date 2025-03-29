#include "key_utils.h"

#include "FourQ.h"
#include "k12.h"

#include <address.h>

bool get_subseed_from_seed(const uint8_t *seed, uint8_t *subseed) {
    uint8_t seedBytes[55];
    for (int i = 0; i < 55; i++) {
        if (seed[i] < 'a' || seed[i] > 'z') {
            return false;
        }
        seedBytes[i] = seed[i] - 'a';
    }
    kangaroo_twelve(seedBytes, sizeof(seedBytes), subseed, SUBSEED_LENGTH);

    return true;
}
