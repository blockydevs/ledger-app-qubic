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

#ifdef HAVE_BAGL

#include <stdbool.h>
#include <string.h>

#include "os.h"
#include "ux.h"
#include "glyphs.h"
#include "io.h"
#include "format.h"

#include "ui_api.h"
#include "../globals.h"
#include "../sw.h"
#include "../address.h"
#include "action/validate.h"

static action_validate_cb g_validate_callback;
static char g_address[PUBLIC_IDENTITY_LENGTH + 1];

UX_STEP_NOCB(ux_display_confirm_addr_step, pn, {&C_icon_eye, "Confirm Address"});
UX_STEP_NOCB(ux_display_address,
             bnnn_paging,
             {
                 .title = "Address",
                 .text = g_address,
             });
UX_STEP_CB(ux_display_approve,
           pb,
           (*g_validate_callback)(true),
           {
               &C_icon_validate_14,
               "Approve",
           });
UX_STEP_CB(ux_display_reject,
           pb,
           (*g_validate_callback)(false),
           {
               &C_icon_crossmark,
               "Reject",
           });
UX_FLOW(ux_display_pubkey_flow,
        &ux_display_confirm_addr_step,
        &ux_display_address,
        &ux_display_approve,
        &ux_display_reject);

static void ui_action_validate_pubkey(bool choice) {
    validate_pubkey(choice);
    ui_menu_main();
}

int ui_display_address() {
    if (G_context.req_type != CONFIRM_ADDRESS || G_context.state != STATE_NONE) {
        G_context.state = STATE_NONE;
        return io_send_sw(SW_BAD_STATE);
    }
    memset(g_address, 0, sizeof(g_address));

    // Produce Qubic public ID
    get_identity_from_public_key(G_context.pk_info.raw_public_key, g_address, false);

    g_validate_callback = &ui_action_validate_pubkey;

    ux_flow_init(0, ux_display_pubkey_flow, NULL);
    return 0;
}

#endif
