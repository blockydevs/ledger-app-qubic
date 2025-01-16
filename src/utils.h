
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t

#include "globals.h"
#include "lib_standard_app/crypto_helpers.h"

int encode_base26(const void *in, size_t length, char *out, size_t maxoutlen);
void get_public_key(uint8_t publicKeyArray[static PUBKEY_LENGTH],
                    const uint32_t *derivationPath,
                    size_t pathLength);

#endif //UTILS_H
