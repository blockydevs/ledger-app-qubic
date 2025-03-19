#pragma once

#include "types.h"

#define PUBKEY_LENGTH 32

/**
 * Global context for user requests.
 */
extern global_ctx_t G_context;

/**
 * Global structure for NVM data storage.
 */

// display stepped screens
extern unsigned int ux_step;
extern unsigned int ux_step_count;

enum BlindSign {
    BlindSignDisabled = 0,
    BlindSignEnabled = 1,
};

enum PubkeyDisplay {
    PubkeyDisplayLong = 0,
    PubkeyDisplayShort = 1,
};

enum DisplayMode {
    DisplayModeUser = 0,
    DisplayModeExpert = 1,
};

typedef struct AppSettings {
    uint8_t allow_blind_sign;
    uint8_t pubkey_display;
    uint8_t display_mode;
} AppSettings;

typedef struct internal_storage_t {
    AppSettings settings;
    uint8_t initialized;
} internal_storage_t;

extern const internal_storage_t N_storage_real;
#define N_storage (*(volatile internal_storage_t *) PIC(&N_storage_real))
