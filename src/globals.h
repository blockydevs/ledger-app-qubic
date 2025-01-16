#pragma once

#include "ux.h"

#include "io.h"
#include "types.h"

#define PUBKEY_LENGTH 32

/**
 * Global buffer for interactions between SE and MCU.
 */
extern uint8_t G_io_seproxyhal_spi_buffer[IO_SEPROXYHAL_BUFFER_SIZE_B];

/**
 * Global structure to perform asynchronous UX aside IO operations.
 */
extern ux_state_t G_ux;

/**
 * Global structure with the parameters to exchange with the BOLOS UX application.
 */
extern bolos_ux_params_t G_ux_params;

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
