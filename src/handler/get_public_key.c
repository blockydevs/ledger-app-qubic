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
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <string.h>   // memset, explicit_bzero

#include "os.h"
#include "buffer.h"


#include "get_public_key.h"

#include "address.h"
#include "fp_arm.h"
#include "../globals.h"
#include "../types.h"
#include "../ui/ui_api.h"
#include "../helper/send_response.h"
#include "k12.h"
#include "key_utils.h"
#include "sw.h"
#include "lib_standard_app/crypto_helpers.h"

int handler_get_public_key(buffer_t *cdata, const bool display) {
    explicit_bzero(&G_context, sizeof(G_context));
    G_context.req_type = CONFIRM_ADDRESS;
    G_context.state = STATE_NONE;

    if (!buffer_read_u8(cdata, &G_context.bip32_path_len) ||
        !buffer_read_bip32_path(cdata,
                                G_context.bip32_path,
                                (size_t) G_context.bip32_path_len)) {
        return io_send_sw(SW_WRONG_DATA_LENGTH);
    }


    // Allocate 64 bytes to respect Syscall API but only 32 will be used
    cx_ecfp_256_private_key_t raw_private_key = {0};
    uint8_t chain_code[40] = {0};
    char seed[SEED_LENGTH] = {0};
    char publicIdentity[128] = {0};

    const cx_err_t cx_err_priv = bip32_derive_with_seed_init_privkey_256(
        HDW_NORMAL,
        CX_CURVE_SECP256K1,
        G_context.bip32_path,
        G_context.bip32_path_len,
        &raw_private_key,
        chain_code,
        NULL,
        0
    );

    if (CX_OK != cx_err_priv) {
        THROW(cx_err_priv);
    }

    PRINTF("Derivation path length: %d\n", G_context.bip32_path_len);
    PRINTF("APDU derivation path:\n");
    for (int j = 0; j < G_context.bip32_path_len; j++) {
        PRINTF("%d", G_context.bip32_path[j]);
        PRINTF("/");
    }
    PRINTF("\n");

    PRINTF("Raw private key (ledger):\n");
    for (int j = 0; j < 32; j++) {
        PRINTF("%02X", raw_private_key.d[j]);
    }
    PRINTF("\n");

    PRINTF("Chain code: \n");
    for (int j = 0; j < 40; j++) {
        PRINTF("%02X", chain_code[j]);
    }
    PRINTF("\n");

    PRINTF("Generating Qubic seed from derived BIP32 key\n");

    internal_key_to_seed(raw_private_key.d, seed);

    PRINTF("Qubic seed: \n");
    for (int j = 0; j < 55; j++) {
        PRINTF("%c", seed[j]);
    }
    PRINTF("\n");

    uint8_t subseed[32] = {0};
    get_subseed_from_seed(seed, subseed);

    PRINTF("Subseed value:\n");
    for (int j = 0; j < 32; j++) {
        PRINTF("%02X", subseed[j]);
    }
    PRINTF("\n");

    uint8_t publicKey[32] = {0};
    SchnorrQ_KeyGeneration(subseed, publicKey);

    PRINTF("Public key: ");
    for (int j = 0; j < 32; j++) {
        PRINTF("%02X", publicKey[j]);
    }
    PRINTF("\n");

    get_identity_from_public_key(publicKey, publicIdentity, false);

    PRINTF("Derived identity: ");
    for (int j = 0; j < 60; j++) {
        PRINTF("%c", publicIdentity[j]);
    }
    PRINTF("\n");

    memcpy(G_context.pk_info.raw_public_key, publicIdentity, 60);

    if (display) {
        return ui_display_address();
    }

    return helper_send_response_pubkey();
}
