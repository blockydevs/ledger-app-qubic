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

#include "address.h"
#ifdef HAVE_BAGL

#include <stdbool.h>
#include <string.h>

#include "os.h"
#include "ux.h"
#include "glyphs.h"
#include "io.h"
#include "printer.h"

#include "ui_api.h"
#include "../globals.h"
#include "action/validate.h"
#include "../transaction/types.h"

static action_validate_cb g_validate_callback;
static char g_amount[30];
static char g_address[65];
static char g_tick[30];
static char g_public_key[65];

UX_STEP_NOCB(ux_display_public_key_step,
             bnnn_paging,
             {
             .title = "From address",
             .text = g_public_key,
             });
UX_STEP_NOCB(ux_display_address_step,
             bnnn_paging,
             {
             .title = "To address",
             .text = g_address,
             });
UX_STEP_CB(ux_display_approve_step,
           pb,
           (*g_validate_callback)(true),
           {
           &C_icon_validate_14,
           "Approve",
           });
UX_STEP_CB(ux_display_reject_step,
           pb,
           (*g_validate_callback)(false),
           {
           &C_icon_crossmark,
           "Reject",
           });
UX_STEP_NOCB(ux_display_blind_sign_warning_step,
             bnnn_paging,
             {
             .title = "Blind sign ahead",
             .text = "The details of this transaction or message are not fully verifiable. "
             "If you sign it, you could lose all your assets.",
             });
UX_STEP_NOCB(ux_display_review_step,
             pnn,
             {
             &C_icon_eye,
             "Review",
             "Transaction",
             });
UX_STEP_NOCB(ux_display_amount_step,
             bnnn_paging,
             {
             .title = "Amount",
             .text = g_amount,
             });
UX_STEP_NOCB(ux_display_tick_step,
             bnnn_paging,
             {
             .title = "Tick",
             .text = g_tick,
             });

UX_FLOW(ux_display_transaction_flow,
        &ux_display_review_step,
        &ux_display_public_key_step,
        &ux_display_address_step,
        &ux_display_amount_step,
        &ux_display_approve_step,
        &ux_display_reject_step);

UX_FLOW(ux_display_expert_transaction_flow,
        &ux_display_review_step,
        &ux_display_public_key_step,
        &ux_display_address_step,
        &ux_display_amount_step,
        &ux_display_tick_step,
        &ux_display_approve_step,
        &ux_display_reject_step);

UX_FLOW(ux_display_blind_sign_transaction_flow,
        &ux_display_blind_sign_warning_step,
        &ux_display_approve_step,
        &ux_display_reject_step);

static void ui_action_validate_transaction(bool choice) {
    validate_transaction(choice);
    ui_menu_main();
}

int ui_display_transaction() {
    get_identity_from_public_key(G_context.tx_info.transaction_qubic.destination_public_key, g_address, false);

    g_address[64] = '\0';

    get_identity_from_public_key(G_context.tx_info.transaction_qubic.source_public_key, g_public_key, false);
    g_public_key[64] = '\0';

    print_u64(G_context.tx_info.transaction_qubic.amount, g_amount, sizeof(g_amount));
    print_u64(G_context.tx_info.transaction_qubic.tick, g_tick, sizeof(g_tick));

    g_validate_callback = &ui_action_validate_transaction;

    if (N_storage.settings.display_mode == DisplayModeExpert) {
        ux_flow_init(0, ux_display_expert_transaction_flow, NULL);
    } else {
        ux_flow_init(0, ux_display_transaction_flow, NULL);
    }

    return 0;
}

int ui_display_blind_signed_transaction(void) {
    g_validate_callback = &ui_action_validate_transaction;
    ux_flow_init(0, ux_display_blind_sign_transaction_flow, NULL);
    return 0;
}

#endif
