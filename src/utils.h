#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>  // uint*_t
#include <stddef.h>  // size_t

int encode_base26(const uint8_t *hex_input,
                  size_t input_length,
                  char *output,
                  const size_t output_size);

int pad_result_with_z(char *target_string, size_t target_length);

#endif  // UTILS_H
