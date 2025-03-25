#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <string.h>   // memset, explicit_bzero

#include "os.h"
#include "io.h"
#include "buffer.h"

#include "get_public_key.h"

#include "address.h"
#include "qubic_keygen.h"
#include "../globals.h"
#include "../types.h"
#include "../ui/ui_api.h"
#include "../helper/send_response.h"
#include "sw.h"

int handler_get_public_key(buffer_t *cdata, const bool display) {
    explicit_bzero(&G_context, sizeof(G_context));
    G_context.req_type = CONFIRM_ADDRESS;
    G_context.state = STATE_NONE;

    PRINTF("Parsing BIP32 path\n");
    if (!buffer_read_u8(cdata, &G_context.bip32_path_len) ||
        !buffer_read_bip32_path(cdata, G_context.bip32_path, (size_t) G_context.bip32_path_len)) {
        return io_send_sw(SW_WRONG_DATA_LENGTH);
    }

    // One byte is used to represent the amount of path chunks;
    const uint8_t derivation_path_length = G_context.bip32_path_len * sizeof(uint32_t) + 1;
    if (cdata->size < derivation_path_length) {
        return io_send_sw(SW_WRONG_DATA_LENGTH);
    }

    PRINTF("Derivation path length: %d\n", G_context.bip32_path_len);

    qubic_keypair_t qubic_keypair = {0};
    derive_qubic_keypair(&qubic_keypair);

    memcpy(G_context.pk_info.raw_public_key, qubic_keypair.public_key, PUBKEY_LENGTH);
    explicit_bzero(qubic_keypair.public_key, PUBKEY_LENGTH);
    explicit_bzero(qubic_keypair.subseed, SUBSEED_LENGTH);

    if (display) {
        return ui_display_address();
    }

    return helper_send_response_pubkey();
}
