#include QMK_KEYBOARD_H
#define CLIPBOARD_SIZE 512
#define DELAY_BETWEEN_CHARS 2
#define RAPIDFIRE_INTERVAL 50 // 20 clicks per second

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  TRIPLE_TAP = 5,
  TRIPLE_HOLD = 6
};

enum {
  ALT_OSL1 = 0,
};

enum dumb_keycodes {
  TAUNTTXT = SAFE_RANGE,
  YANK_V,
  RAPIDF,
};

bool tAuNtTeXt = false;
char clipboard[CLIPBOARD_SIZE] = {0};
uint16_t clipboard_idx = 0;
bool clipboard_read_until_null = false;
bool rapidfire = false;
uint32_t rapidfire_timer = 0;

int cur_dance (tap_dance_state_t *state);
void alt_finished (tap_dance_state_t *state, void *user_data);
void alt_reset (tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
		[0] = LAYOUT_65_ansi_blocker( 
			QK_GESC, KC_1, KC_2,     KC_3,  KC_4,  KC_5,  KC_6,    KC_7,  KC_8,    KC_9,     KC_0,     KC_MINS,  KC_EQL,  KC_BSPC,   KC_HOME,
		    KC_TAB,  KC_Q,     KC_W,     KC_E,  KC_R,  KC_T,  KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,     KC_LBRC,  KC_RBRC, KC_BSLS, KC_PGUP,
			KC_CAPS, KC_A,     KC_S,     KC_D,  KC_F,  KC_G,  KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN,  KC_QUOT,           KC_ENT,    KC_PGDN,
		    KC_LSFT, KC_Z,     KC_X,     KC_C,  KC_V,  KC_B,  KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH,  KC_RSFT,           KC_UP,     KC_END,
		    KC_LCTL, KC_LGUI,  TD(ALT_OSL1),           KC_SPC,                         MO(1),    KC_RALT,    KC_LEFT,           KC_DOWN,   KC_RIGHT),
		[1] = LAYOUT_65_ansi_blocker( 
			KC_GRV,   KC_F1,    KC_F2,   KC_F3,  KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,  KC_DEL,   KC_INS,
		    KC_TRNS,  RGB_TOG,  RGB_MOD, RGB_HUI,RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_PSCR, KC_SCRL,  KC_PAUS, QK_BOOT,  KC_PGUP,
			TAUNTTXT, RGB_SPI,  RGB_SPD, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  RAPIDF,          EE_CLR,   KC_PGDN,
		    KC_LSFT,  KC_TRNS,  KC_TRNS, KC_F21, YANK_V,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,          KC_VOLU,  KC_MUTE,
		    KC_TRNS,  KC_TRNS,  TD(ALT_OSL1),             KC_TRNS,                   KC_TRNS,          KC_TRNS,  KC_MPRV,          KC_VOLD,  KC_MNXT),
};

void matrix_scan_user(void) {
  if (rapidfire && timer_elapsed32(rapidfire_timer) >= RAPIDFIRE_INTERVAL) {
    tap_code(KC_MS_BTN1);
    rapidfire_timer = timer_read32();
  }
}

int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->pressed) return SINGLE_HOLD;
    else return SINGLE_TAP;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8;
}

static tap alttap_state = {
  .is_press_action = true,
  .state = 0
};

void alt_finished (tap_dance_state_t *state, void *user_data) {
  alttap_state.state = cur_dance(state);
  switch (alttap_state.state) {
    case SINGLE_TAP: set_oneshot_layer(1, ONESHOT_START); clear_oneshot_layer_state(ONESHOT_PRESSED); break;
    case SINGLE_HOLD: register_code(KC_LALT); break;
    case DOUBLE_TAP: set_oneshot_layer(1, ONESHOT_START); set_oneshot_layer(1, ONESHOT_PRESSED); break;
    case DOUBLE_HOLD: register_code(KC_LALT); layer_on(1); break;
    //Last case is for fast typing. Assuming your key is `f`:
    //For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
    //In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
  }
}

void alt_reset (tap_dance_state_t *state, void *user_data) {
  switch (alttap_state.state) {
    case SINGLE_TAP: break;
    case SINGLE_HOLD: unregister_code(KC_LALT); break;
    case DOUBLE_TAP: break;
    case DOUBLE_HOLD: layer_off(1); unregister_code(KC_LALT); break;
  }
  alttap_state.state = 0;
}

tap_dance_action_t tap_dance_actions[] = {
  [ALT_OSL1]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_finished, alt_reset)
};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_TRNS:
    case KC_NO:
      /* Always cancel one-shot layer when another key gets pressed */
      if (record->event.pressed && is_oneshot_layer_active())
      clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
      return true;
    case QK_BOOT:
      /* Don't allow reset from oneshot layer state */
      if (record->event.pressed && is_oneshot_layer_active()){
        clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
        return false;
      }
      return true;
    default:
      return true;
  }
  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (tAuNtTeXt) {
    if (record->event.pressed) {
      if (keycode != KC_SPC) {
        register_code(KC_CAPS);
        unregister_code(KC_CAPS);
      }
    }
  }

  switch (keycode) {
    case TAUNTTXT:
      if (record->event.pressed) {
        tAuNtTeXt = !tAuNtTeXt;
        // when it's turned on, toggle caps lock (makes first letter lowercase)
        if (tAuNtTeXt) {
          register_code(KC_CAPS);
          unregister_code(KC_CAPS);
        }
      }
      return false;
    case YANK_V:
      // test code
      if (record->event.pressed) {
        send_string(clipboard);
      }
      return false;
    case RAPIDF:
      if (record->event.pressed) {
          rapidfire = !rapidfire;
          rapidfire_timer = timer_read32();
      }
      return true;

    default:
      return true; //Process all other keycodes normally
    }
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    for (uint16_t i = 0; i < length; i++) {
        if (clipboard_read_until_null) {
            // CLIPBOARD IS FULL, IGNORE EVERYTHING ELSE SENT FOR CURRENT STRING
            // The corresponding Python script has guards but I just want to be
            // on the err of caution ¯\_(ツ)_/¯.
            if (data[i] == '\00') {
                clipboard_read_until_null = false;
                break;
            }
            continue;
        }

        clipboard[clipboard_idx] = data[i];
        if (data[i] == '\00') {
            // Null terminator received, set the cursor back to 0 for next yank
            clipboard_idx = 0;
            break;
        } else {
            clipboard_idx++;
            if (clipboard_idx == CLIPBOARD_SIZE) {
                // Clipboard is full: defensive coding to prevent buffer overflow
                clipboard[CLIPBOARD_SIZE - 1] = '\00';
                clipboard_read_until_null = true;
                clipboard_idx = 0;
                break;
            }
        }
    }
}
