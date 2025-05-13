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

#ifdef HAVE_NBGL

#include <stdbool.h>
#include <string.h>

#include "address.h"
#include "os.h"
#include "glyphs.h"
#include "os_io_seproxyhal.h"
#include "nbgl_use_case.h"
#include "io.h"
#include "bip32.h"
#include "format.h"
#include "printer.h"

#include "ui_api.h"
#include "constants.h"
#include "../globals.h"
#include "../sw.h"
#include "action/validate.h"
#include "../transaction/types.h"

static char g_amount[30];
static char g_address[65];
static char g_tick[30];
static char g_public_key[65];

static nbgl_layoutTagValue_t pairs[4];
static nbgl_layoutTagValueList_t pairList;

static void review_choice(bool confirm) {
    validate_transaction(confirm);
    if (confirm) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_menu_main);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
    }
}

int ui_display_transaction_bs_choice(bool is_blind_signed) {
    memset(g_amount, 0, sizeof(g_amount));
    char amount[30] = {0};
    if (!format_fpu64(amount,
                      sizeof(amount),
                      G_context.tx_info.transaction_qubic.amount,
                      EXPONENT_SMALLEST_UNIT)) {
        return io_send_sw(SW_DISPLAY_AMOUNT_FAIL);
    }
    memset(g_address, 0, sizeof(g_address));

    get_identity_from_public_key(G_context.tx_info.transaction_qubic.destination_public_key,
                                 g_address,
                                 false);
    g_address[64] = '\0';

    get_identity_from_public_key(G_context.tx_info.transaction_qubic.source_public_key,
                                 g_public_key,
                                 false);
    g_public_key[64] = '\0';

    print_u64(G_context.tx_info.transaction_qubic.amount, g_amount, sizeof(g_amount));
    print_u64(G_context.tx_info.transaction_qubic.tick, g_tick, sizeof(g_tick));

    pairs[0].item = "Amount";
    pairs[0].value = g_amount;
    pairs[1].item = "From address";
    pairs[1].value = g_public_key;
    pairs[2].item = "To address";
    pairs[2].value = g_address;

    pairList.nbMaxLinesForValue = 0;

    if (N_storage.settings.display_mode == DisplayModeExpert) {
        PRINTF("Display Mode Expert\n");
        pairs[3].item = "Tick";
        pairs[3].value = g_tick;
        pairList.nbPairs = 4;
    } else {
        pairList.nbPairs = 3;
    }

    pairList.pairs = pairs;

    if (is_blind_signed) {
        nbgl_useCaseReviewBlindSigning(TYPE_TRANSACTION,
                                       &pairList,
                                       &C_app_qubic_64px,
                                       "Review transaction\nto send QUBIC",
                                       NULL,
                                       "Sign transaction\nto send QUBIC",
                                       NULL,
                                       review_choice);
    } else {
        nbgl_useCaseReview(TYPE_TRANSACTION,
                           &pairList,
                           &C_app_qubic_64px,
                           "Review transaction\nto send QUBIC",
                           NULL,
                           "Sign transaction\nto send QUBIC",
                           review_choice);
    }

    return 0;
}

int ui_display_blind_signed_transaction(void) {
    return ui_display_transaction_bs_choice(true);
}

int ui_display_transaction() {
    return ui_display_transaction_bs_choice(false);
}

#endif
