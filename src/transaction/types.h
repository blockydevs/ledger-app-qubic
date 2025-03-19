#pragma once

#include <stdint.h>  // uint*_t

#define MAX_TX_LEN   510
#define ADDRESS_LEN  32
#define MAX_MEMO_LEN 465  // 510 - ADDRESS_LEN - 2*SIZE(U64) - SIZE(MAX_VARINT)

typedef enum {
    PARSING_OK = 1,
    VALUE_PARSING_ERROR = -2,
    WRONG_LENGTH_ERROR = -3,
    WRONG_DESTINATION_ADDRESS_ERROR = -4,
    WRONG_SOURCE_ADDRESS_ERROR = -5
} parser_status_e;

typedef struct {
    uint8_t source_public_key[ADDRESS_LEN];
    uint8_t destination_public_key[ADDRESS_LEN];
    uint64_t amount;
    uint32_t tick;
    uint16_t inputType;
    uint16_t inputSize;
} transaction_qubic_t;
