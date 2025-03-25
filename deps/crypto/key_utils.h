#ifndef KEY_UTILS_H
#define KEY_UTILS_H

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool

bool get_subseed_from_seed(const uint8_t *seed, uint8_t *subseed);

#endif  // KEY_UTILS_H
