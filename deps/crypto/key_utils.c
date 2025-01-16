#include "key_utils.h"

#include "FourQ.h"
#include "FourQ_api.h"
#include "FourQ_internal.h"
#include "k12.h"

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

// void get_private_key_from_sub_seed(const uint8_t *subseed, uint8_t *privateKey) {
//     kangaroo_twelve(subseed, SUBSEED_LENGTH, privateKey, SUBSEED_LENGTH);
// }
//
// void get_public_key_from_private_key(const uint8_t* privateKey, uint8_t* publicKey)
// {
//     point_t P;
//     ecc_mul_fixed((unsigned long long*)privateKey, P); // Compute public key
//     encode(P, publicKey);
// }