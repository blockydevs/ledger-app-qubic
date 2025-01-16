/*****************************************************************************
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "os.h"
#include "cx.h"
#include "ledger_assert.h"

#include "address.h"

#include "k12.h"
#include "util.h"
#include "transaction/types.h"
#include "utils.h"

/**
 * Calculate Identity
 * @param public_key
 * @param out
 * @param out_len
 * @return
 */
bool address_from_pubkey(const uint8_t public_key[static 65], uint8_t *out, size_t out_len) {
    uint8_t address[32] = {0};

    LEDGER_ASSERT(out != NULL, "NULL out");

    if (out_len < PUBLIC_IDENTITY_LENGTH) {
        return false;
    }

    if (cx_keccak_256_hash(public_key + 1, 64, address) != CX_OK) {
        return false;
    }

    // memmove(out, address + sizeof(address) - ADDRESS_LEN, ADDRESS_LEN);
    memmove(out, public_key, PUBLIC_IDENTITY_LENGTH);

    return true;
}

/**
 *
 * @param internal_private_key
 * @param out Output buffer (always 55 bytes)
 * @return
 */
bool internal_key_to_seed(const uint8_t internal_private_key[static 32], char *out) {
    char seed[SEED_LENGTH] = {0};

    //convert to base 26
    BAIL_IF(encode_base26(internal_private_key,sizeof(internal_private_key), seed, SEED_LENGTH));

    //pad seed to 55 bytes
    const size_t seed_length = strnlen(seed, SEED_LENGTH);

    const size_t pad_length = 55 - seed_length;

    memset(out, 'z', pad_length);

    strncpy(out + pad_length, seed, seed_length);

    return true;
}

void get_identity_from_public_key(const uint8_t *pubkey, char *dstIdentity, bool isLowerCase) {
    uint8_t publicKey[32];
    memcpy(publicKey, pubkey, 32);
    uint16_t identity[61] = {0};
    for (int i = 0; i < 4; i++) {
        unsigned long long publicKeyFragment = *((unsigned long long *) &publicKey[i << 3]);
        for (int j = 0; j < 14; j++) {
            identity[i * 14 + j] = publicKeyFragment % 26 + (isLowerCase ? L'a' : L'A');
            publicKeyFragment /= 26;
        }
    }
    unsigned int identityBytesChecksum;
    kangaroo_twelve(publicKey, 32, (uint8_t *) &identityBytesChecksum, 3);
    identityBytesChecksum &= 0x3FFFF;
    for (int i = 0; i < 4; i++) {
        identity[56 + i] = identityBytesChecksum % 26 + (isLowerCase ? L'a' : L'A');
        identityBytesChecksum /= 26;
    }
    identity[60] = 0;
    for (int i = 0; i < 60; i++) dstIdentity[i] = identity[i];
}
