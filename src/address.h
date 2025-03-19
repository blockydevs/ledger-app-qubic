#pragma once

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t


#define SEED_LENGTH 55
#define SUBSEED_LENGTH 55
#define PUBLIC_IDENTITY_LENGTH 60

uint8_t internal_key_to_seed(const uint8_t *internal_private_key, size_t input_length, char *out);

void get_identity_from_public_key(const uint8_t *pubkey, char *dstIdentity, bool isLowerCase);
