#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool


#define SEED_LENGTH 55
#define PUBLIC_IDENTITY_LENGTH 60

/**
 * Convert public key to address.
 *
 * address = Keccak256(public_key)[12:32] (20 bytes)
 *
 * @param[in]  public_key
 *   Pointer to byte buffer with public key.
 *   The public key is represented as 65 bytes with 1 byte for format and 32 bytes for
 *   each coordinate.
 * @param[out] out
 *   Pointer to output byte buffer for address.
 * @param[in]  out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool address_from_pubkey(const uint8_t public_key[static 65], uint8_t *out, size_t out_len);

bool internal_key_to_seed(const uint8_t internal_private_key[static 32], char *out);

void get_identity_from_public_key(const uint8_t *pubkey, char *dstIdentity, bool isLowerCase);
