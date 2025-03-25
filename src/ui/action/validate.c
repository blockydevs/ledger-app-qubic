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

#include "io.h"

#include "validate.h"

#include "address.h"
#include "qubic_keygen.h"
#include "FourQ_api.h"
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

    qubic_keypair_t qubic_keypair = {0};
    derive_qubic_keypair(&qubic_keypair);

    if (N_storage.settings.allow_blind_sign != BlindSignEnabled &&
        memcmp(qubic_keypair.public_key,
               G_context.tx_info.transaction_qubic.source_public_key,
               PUBKEY_LENGTH) != 0) {
        PRINTF("Invalid public key\n");
        return -1;
    }

    const ECCRYPTO_STATUS error = SchnorrQ_Sign(qubic_keypair.subseed,
                                                qubic_keypair.public_key,
                                                G_context.tx_info.m_hash,
                                                sizeof(G_context.tx_info.m_hash),
                                                G_context.tx_info.signature);

    if (error != ECCRYPTO_SUCCESS) {
        return -1;
    }

    PRINTF("Signature: %.*H\n", sig_len, G_context.tx_info.signature);

    G_context.tx_info.signature_len = sig_len;
    explicit_bzero(qubic_keypair.public_key, PUBKEY_LENGTH);
    explicit_bzero(qubic_keypair.subseed, SUBSEED_LENGTH);
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
