
#include "utils.h"

void get_public_key(uint8_t publicKeyArray[static PUBKEY_LENGTH],
                    const uint32_t *derivationPath,
                    size_t pathLength) {
    uint8_t rawPubkey[65];
    cx_err_t cx_err;

    //@TODO add Qubic flow here

    cx_err = bip32_derive_with_seed_get_pubkey_256(HDW_NORMAL,
                                                   CX_CURVE_SECP256K1,
                                                   derivationPath,
                                                   pathLength,
                                                   rawPubkey,
                                                   NULL,
                                                   CX_SHA512,
                                                   NULL,
                                                   0);

    if (CX_OK != cx_err) {
        THROW(cx_err);
    }

    for (int i = 0; i < PUBKEY_LENGTH; i++) {
        publicKeyArray[i] = rawPubkey[PUBKEY_LENGTH];
    }
    if ((rawPubkey[PUBKEY_LENGTH] & 1) != 0) {
        publicKeyArray[PUBKEY_LENGTH - 1] |= 0x80;
    }
}

static const char BASE26_ALPHABET[] = {'a', 'b', 'c',
                                       'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm', 'n', 'o', 'p',
                                       'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int encode_base26(const void *in, size_t length, char *out, size_t maxoutlen) {
    uint8_t tmp[64];
    uint8_t buffer[64];
    uint8_t j;
    size_t start_at;
    size_t zero_count = 0;
    if (length > sizeof(tmp)) {
        return INVALID_PARAMETER;
    }
    memmove(tmp, in, length);
    while ((zero_count < length) && (tmp[zero_count] == 0)) {
        ++zero_count;
    }
    j = 2 * length;
    start_at = zero_count;
    while (start_at < length) {
        uint16_t remainder = 0;
        size_t div_loop;
        for (div_loop = start_at; div_loop < length; div_loop++) {
            uint16_t digit256 = (uint16_t) (tmp[div_loop] & 0xff);
            uint16_t tmp_div = remainder * 256 + digit256;
            tmp[div_loop] = (uint8_t) (tmp_div / 26);
            remainder = (tmp_div % 26);
        }
        if (tmp[start_at] == 0) {
            ++start_at;
        }
        buffer[--j] = (uint8_t) BASE26_ALPHABET[remainder];
    }
    while ((j < (2 * length)) && (buffer[j] == BASE26_ALPHABET[0])) {
        ++j;
    }
    while (zero_count-- > 0) {
        buffer[--j] = BASE26_ALPHABET[0];
    }
    length = 2 * length - j;
    if (maxoutlen < length + 1) {
        return EXCEPTION_OVERFLOW;
    }
    memmove(out, (buffer + j), length);
    out[length] = '\0';
    return 0;
}