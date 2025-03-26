/**
 * Code in this file is partially migrated from the Qubic repository
 * (https://github.com/qubic/qubic-cli/blob/main/keyUtils.cpp)
 */

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "address.h"

#include "globals.h"
#include "k12.h"
#include "utils.h"

#include <util.h>

/**
 *
 * @param internal_private_key
 * @param input_length
 * @param out Output buffer (always 55 bytes)
 * @return
 */
uint8_t internal_key_to_seed(const uint8_t *internal_private_key,
                             const size_t input_length,
                             char *out) {
    BAIL_IF(encode_base26(internal_private_key, input_length, out, SEED_LENGTH));

    pad_result_with_z(out, SEED_LENGTH);

    return 0;
}

void get_identity_from_public_key(const uint8_t *pubkey, char *dstIdentity, bool isLowerCase) {
    uint8_t publicKey[PUBKEY_LENGTH];
    memcpy(publicKey, pubkey, PUBKEY_LENGTH);
    uint16_t identity[61] = {0};
    for (int i = 0; i < 4; i++) {
        unsigned long long publicKeyFragment = *((unsigned long long *) &publicKey[i << 3]);
        for (int j = 0; j < 14; j++) {
            identity[i * 14 + j] = publicKeyFragment % 26 + (isLowerCase ? L'a' : L'A');
            publicKeyFragment /= 26;
        }
    }
    unsigned int identityBytesChecksum;
    kangaroo_twelve(publicKey, PUBKEY_LENGTH, (uint8_t *) &identityBytesChecksum, 3);
    identityBytesChecksum &= 0x3FFFF;
    for (int i = 0; i < 4; i++) {
        identity[56 + i] = identityBytesChecksum % 26 + (isLowerCase ? L'a' : L'A');
        identityBytesChecksum /= 26;
    }
    identity[60] = 0;
    for (int i = 0; i < 60; i++) dstIdentity[i] = identity[i];
}
