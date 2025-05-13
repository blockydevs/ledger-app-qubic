#pragma once

#include <stdint.h>  // uint*_t
#include "../globals.h"
#include "../address.h"

typedef struct {
    uint8_t public_key[PUBKEY_LENGTH];
    uint8_t subseed[SUBSEED_LENGTH];
} qubic_keypair_t;

uint8_t derive_qubic_keypair(const qubic_keypair_t *qubic_keypair);