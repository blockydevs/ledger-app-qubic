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

#include <globals.h>

parser_status_e deserialize_transaction(const buffer_t *buf) {
    if (buf == NULL || buf->size > MAX_TX_LEN || buf->size != sizeof(transaction_qubic_t)) {
        return WRONG_LENGTH_ERROR;
    }

    memcpy(&G_context.tx_info.transaction_qubic, buf->ptr, sizeof(transaction_qubic_t));

    int destination_pkey_counter = 0;
    for (int i = 0; (unsigned) i < sizeof(G_context.tx_info.transaction_qubic.destinationPublicKey); i++) {
        if (G_context.tx_info.transaction_qubic.destinationPublicKey[i] == 0) {
            destination_pkey_counter++;
        }
    }

    if (destination_pkey_counter ==
        sizeof(G_context.tx_info.transaction_qubic.destinationPublicKey)) {
        return WRONG_DESTINATION_ADDRESS_ERROR;
    }

    if (G_context.tx_info.transaction_qubic.amount == 0) {
        return VALUE_PARSING_ERROR;
    }

    PRINTF("req_type %d\n", G_context.req_type);

    PRINTF("sourcePublicKey: ");
    for (int i = 0; i < 32; i++) {
        PRINTF("%02x", G_context.tx_info.transaction_qubic.sourcePublicKey[i]);
    }
    PRINTF("\n");

    PRINTF("destinationPublicKey: ");
    for (int i = 0; i < 32; i++) {
        PRINTF("%02x", G_context.tx_info.transaction_qubic.destinationPublicKey[i]);
    }
    PRINTF("\n");

    PRINTF("inputType %d\n", G_context.tx_info.transaction_qubic.inputType);
    PRINTF("inputSize %d\n", G_context.tx_info.transaction_qubic.inputSize);
    PRINTF("amount %d\n", G_context.tx_info.transaction_qubic.amount);
    PRINTF("tick %d\n", G_context.tx_info.transaction_qubic.tick);

    return PARSING_OK;
}
