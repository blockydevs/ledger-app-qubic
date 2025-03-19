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
#include "buffer.h"

#include "deserialize.h"
#include "types.h"
#include "io.h"

#include <globals.h>

parser_status_e deserialize_transaction(const buffer_t *buf) {
    if (buf == NULL || buf->size > MAX_TX_LEN || (buf->size - buf->offset) != sizeof(transaction_qubic_t)) {
        return WRONG_LENGTH_ERROR;
    }

    memcpy(&G_context.tx_info.transaction_qubic, buf->ptr + buf->offset, sizeof(transaction_qubic_t));

    // Cap the size to ADDRESS_LEN to avoid any risk of overflowing buffers during operations
    if (memcmp(G_context.tx_info.transaction_qubic.destination_public_key,
               (uint8_t[ADDRESS_LEN]){0}, ADDRESS_LEN) == 0) {
        return WRONG_DESTINATION_ADDRESS_ERROR;
    }

    if (memcmp(G_context.tx_info.transaction_qubic.source_public_key,
               (uint8_t[ADDRESS_LEN]){0}, ADDRESS_LEN) == 0) {
        return WRONG_SOURCE_ADDRESS_ERROR;
    }

    if (G_context.tx_info.transaction_qubic.amount == 0) {
        return VALUE_PARSING_ERROR;
    }

#ifdef DEBUG
    PRINTF("req_type %d\n", G_context.req_type);
    PRINTF("Source public key: ");
    for (int i = 0; i < 32; i++) {
        PRINTF("%02x", G_context.tx_info.transaction_qubic.source_public_key[i]);
    }
    PRINTF("\n");

    PRINTF("Destination public key: ");
    for (int i = 0; i < 32; i++) {
        PRINTF("%02x", G_context.tx_info.transaction_qubic.destination_public_key[i]);
    }
    PRINTF("\n");

    PRINTF("Input type %d\n", G_context.tx_info.transaction_qubic.inputType);
    PRINTF("Input size %d\n", G_context.tx_info.transaction_qubic.inputSize);
    PRINTF("Amount %d\n", G_context.tx_info.transaction_qubic.amount);
    PRINTF("Target tick %d\n", G_context.tx_info.transaction_qubic.tick);

#endif
    return PARSING_OK;
}
