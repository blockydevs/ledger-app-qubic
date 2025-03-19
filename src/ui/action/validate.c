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

#include <stdbool.h>  // bool

#include "crypto_helpers.h"
#include "io.h"

#include "validate.h"

#include "address.h"
#include "FourQ_api.h"
#include "key_utils.h"
#include "../../sw.h"
#include "../../globals.h"
#include "../../helper/send_response.h"

void validate_pubkey(bool choice) {
    if (choice) {
        helper_send_response_pubkey();
    } else {
        io_send_sw(SW_DENY);
    }
}

static int crypto_sign_message(void) {
    size_t sig_len = sizeof(G_context.tx_info.signature);


    // Allocate 64 bytes to respect Syscall API but only 32 will be used
    cx_ecfp_256_private_key_t raw_private_key = {0};
    char seed[SEED_LENGTH] = {0};

    const cx_err_t cx_err_priv = bip32_derive_with_seed_init_privkey_256(
        HDW_NORMAL,
        CX_CURVE_SECP256K1,
        G_context.bip32_path,
        G_context.bip32_path_len,
        &raw_private_key,
        NULL,
        NULL,
        0
    );

    if (CX_OK != cx_err_priv) {
        THROW(cx_err_priv);
    }

    PRINTF("Generating Qubic seed from derived BIP32 key\n");
    internal_key_to_seed(raw_private_key.d, sizeof(raw_private_key.d), seed);

    uint8_t subseed[SUBSEED_LENGTH] = {0};
    get_subseed_from_seed(seed, subseed);

    uint8_t public_key[PUBKEY_LENGTH] = {0};
    SchnorrQ_KeyGeneration(subseed, public_key);

    if (N_storage.settings.allow_blind_sign != BlindSignEnabled
        && memcmp(public_key, G_context.tx_info.transaction_qubic.source_public_key, PUBKEY_LENGTH) != 0) {
        PRINTF("Invalid public key\n");
        return -1;
    }

#ifdef DEBUG
    PRINTF("Public key: ");
    for (int j = 0; j < PUBKEY_LENGTH; j++) {
        PRINTF("%02X", public_key[j]);
    }
    PRINTF("\n");
    PRINTF("Transaction source public key: ");
    for (int j = 0; j < PUBKEY_LENGTH; j++) {
        PRINTF("%02X", G_context.tx_info.transaction_qubic.source_public_key[j]);
    }
    PRINTF("\n");
#endif


    ECCRYPTO_STATUS error = SchnorrQ_Sign(subseed, public_key, G_context.tx_info.m_hash,
                                          sizeof(G_context.tx_info.m_hash), G_context.tx_info.signature);

    if (error != ECCRYPTO_SUCCESS) {
        return -1;
    }

    PRINTF("Signature: %.*H\n", sig_len, G_context.tx_info.signature);

    G_context.tx_info.signature_len = sig_len;
    explicit_bzero(public_key, PUBKEY_LENGTH);
    explicit_bzero(seed, SEED_LENGTH);
    explicit_bzero(subseed, SUBSEED_LENGTH);
    return 0;
}

void validate_transaction(bool choice) {
    if (choice) {
        G_context.state = STATE_APPROVED;

        if (crypto_sign_message() != 0) {
            G_context.state = STATE_NONE;
            io_send_sw(SW_SIGNATURE_FAIL);
        } else {
            helper_send_response_sig();
        }
    } else {
        G_context.state = STATE_NONE;
        io_send_sw(SW_DENY);
    }
}
