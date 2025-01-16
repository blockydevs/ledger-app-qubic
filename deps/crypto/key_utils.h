#ifndef KEY_UTILS_H
#define KEY_UTILS_H

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool

#define SUBSEED_LENGTH 32

bool get_subseed_from_seed(const uint8_t *seed, uint8_t *subseed);

// void get_private_key_from_sub_seed(const uint8_t *subseed, uint8_t *privateKey);
// void get_public_key_from_private_key(const uint8_t* privateKey, uint8_t* publicKey);

#endif //KEY_UTILS_H
