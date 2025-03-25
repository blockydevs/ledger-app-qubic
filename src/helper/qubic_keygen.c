#include "qubic_keygen.h"
#include "crypto_helpers.h"
#include "FourQ_api.h"
#include "key_utils.h"

uint8_t derive_qubic_keypair(qubic_keypair_t *qubic_keypair) {
    if (qubic_keypair == NULL) {
        return 1;
    }

    // Allocate 64 bytes to respect Syscall API but only 32 will be used
    cx_ecfp_256_private_key_t raw_private_key = {0};
    char seed[SEED_LENGTH] = {0};

    const cx_err_t cx_err_priv = bip32_derive_with_seed_init_privkey_256(HDW_NORMAL,
                                                                         CX_CURVE_SECP256K1,
                                                                         G_context.bip32_path,
                                                                         G_context.bip32_path_len,
                                                                         &raw_private_key,
                                                                         NULL,
                                                                         NULL,
                                                                         0);

    if (CX_OK != cx_err_priv) {
        THROW(cx_err_priv);
    }

    PRINTF("Generating Qubic seed from derived BIP32 key\n");
    internal_key_to_seed(raw_private_key.d, sizeof(raw_private_key.d), seed);

    get_subseed_from_seed(seed, qubic_keypair->subseed);
    explicit_bzero(seed, SEED_LENGTH);

    SchnorrQ_KeyGeneration(qubic_keypair->subseed, qubic_keypair->public_key);

    return 0;
}