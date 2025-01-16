#ifdef HAVE_BAGL

#include "os.h"
#include "ux.h"
#include "glyphs.h"

#include "../globals.h"
#include "ui_api.h"

void display_settings(void);

void switch_allow_blind_sign_data(void);

void switch_pubkey_display_data(void);

//////////////////////////////////////////////////////////////////////
static const char *settings_submenu_getter(unsigned int idx);

static void settings_submenu_selector(unsigned int idx);

static const char *allow_blind_sign_data_getter(unsigned int idx);

static void allow_blind_sign_data_selector(unsigned int idx);

static const char *display_mode_data_getter(unsigned int idx);

static void display_mode_data_selector(unsigned int idx);

//////////////////////////////////////////////////////////////////////////////////////
// Settings menu:

enum SettingsMenuOption {
    SettingsMenuOptionAllowBlindSign,
    SettingsMenuOptionDisplayMode,
    // back must remain last
    SettingsMenuOptionBack
};

//////////////////////////////////////////////////////////////////////

UX_STEP_NOCB(ux_idle_flow_1_step,
             pnn,
             {
             &C_app_boilerplate_16px,
             "Application",
             "is ready",
             });
UX_STEP_CB(ux_idle_flow_2_step,
           pb,
           ux_menulist_init(0, settings_submenu_getter, settings_submenu_selector),
           {
           &C_icon_coggle,
           "Settings",
           });
UX_STEP_NOCB(ux_idle_flow_3_step,
             bn,
             {
             "Version",
             APPVERSION,
             });
UX_STEP_CB(ux_idle_flow_4_step,
           pb,
           os_sched_exit(-1),
           {
           &C_icon_dashboard_x,
           "Quit",
           });
UX_FLOW(ux_idle_flow,
        &ux_idle_flow_1_step,
        &ux_idle_flow_2_step,
        &ux_idle_flow_3_step,
        &ux_idle_flow_4_step,
        FLOW_LOOP);

//////////////////////////////////////////////////////////////////////

void ui_menu_main(void) {
    // reserve a display stack slot if none yet
    if (G_ux.stack_count == 0) {
        ux_stack_push();
    }
    ux_flow_init(0, ux_idle_flow, NULL);
}

static unsigned int settings_submenu_option_index(enum SettingsMenuOption settings_menu_option) {
    switch (settings_menu_option) {
        case SettingsMenuOptionAllowBlindSign:
        case SettingsMenuOptionDisplayMode:
        default: return settings_menu_option;;
    }
}

const char *const settings_submenu_getter_values[] = {
    "Allow blind sign",
    "Display mode",
    "Back",
};

static const char *settings_submenu_getter(unsigned int idx) {
    if (idx < ARRAYLEN(settings_submenu_getter_values)) {
        return settings_submenu_getter_values[idx];
    }
    return NULL;
}

static void settings_submenu_selector(unsigned int idx) {
    switch (idx) {
        case 0:
            ux_menulist_init_select(0,
                                    allow_blind_sign_data_getter,
                                    allow_blind_sign_data_selector,
                                    N_storage.settings.allow_blind_sign);
            break;
        case 1:
            ux_menulist_init_select(0,
                                    display_mode_data_getter,
                                    display_mode_data_selector,
                                    N_storage.settings.display_mode);
            break;
        default:
            ui_menu_main();
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// Allow blind signing submenu

static void allow_blind_sign_data_change(enum BlindSign blind_sign) {
    uint8_t value;
    switch (blind_sign) {
        case BlindSignDisabled:
        case BlindSignEnabled:
            value = (uint8_t) blind_sign;
            nvm_write((void *) &N_storage.settings.allow_blind_sign, &value, sizeof(value));
            break;
    }
}

const char *const no_yes_data_getter_values[] = {"No", "Yes", "Back"};

static const char *allow_blind_sign_data_getter(unsigned int idx) {
    if (idx < ARRAYLEN(no_yes_data_getter_values)) {
        return no_yes_data_getter_values[idx];
    }
    return NULL;
}

static void allow_blind_sign_data_selector(unsigned int idx) {
    switch (idx) {
        case 0:
            allow_blind_sign_data_change(BlindSignDisabled);
            break;
        case 1:
            allow_blind_sign_data_change(BlindSignEnabled);
            break;
        default:
            break;
    }
    unsigned int select_item = settings_submenu_option_index(SettingsMenuOptionAllowBlindSign);
    ux_menulist_init_select(0, settings_submenu_getter, settings_submenu_selector, select_item);
}


//////////////////////////////////////////////////////////////////////////////////////
// Display mode submenu

static void display_mode_data_change(enum DisplayMode display_mode) {
    uint8_t value;
    switch (display_mode) {
        case DisplayModeUser:
        case DisplayModeExpert:
            value = (uint8_t) display_mode;
            nvm_write((void *) &N_storage.settings.display_mode, &value, sizeof(value));
            break;
    }
}

const char *const display_mode_data_getter_values[] = {"User", "Expert", "Back"};

static const char *display_mode_data_getter(unsigned int idx) {
    if (idx < ARRAYLEN(display_mode_data_getter_values)) {
        return display_mode_data_getter_values[idx];
    }
    return NULL;
}

static void display_mode_data_selector(unsigned int idx) {
    switch (idx) {
        case 0:
            display_mode_data_change(DisplayModeUser);
            break;
        case 1:
            display_mode_data_change(DisplayModeExpert);
            break;
        default:
            break;
    }
    unsigned int select_item = settings_submenu_option_index(SettingsMenuOptionDisplayMode);
    ux_menulist_init_select(0, settings_submenu_getter, settings_submenu_selector, select_item);
}

#endif
