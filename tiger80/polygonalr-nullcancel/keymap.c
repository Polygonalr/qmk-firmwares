/* Copyright 2022 DZTECH <moyi4681@Live.cn>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 #include QMK_KEYBOARD_H

 #define SOCD_OVERLAP_DELAY 16
 #define COUNTERSTRAFE_DELAY 50
 #define PERFECT_INPUTS_BEFORE_DELAY 1
 #define RAPIDFIRE_INTERVAL 50 // 20 clicks per second
 
 enum custom_keycodes {
     ASTF_A = SAFE_RANGE,
     ASTF_D,
     ASTF_W,
     ASTF_S,
     RAPIDF,
 };
 
 enum counterstrafe_state {
     CS_NEUTRAL,
     CS_HOLD,
     CS_DELAY,
     CS_COUNTERSTRAFE,
 };
 
 bool a_down = false;
 bool d_down = false;
 bool w_down = false;
 bool s_down = false;
 uint8_t a_state = CS_NEUTRAL;
 uint8_t d_state = CS_NEUTRAL;
 uint8_t w_state = CS_NEUTRAL;
 uint8_t s_state = CS_NEUTRAL;
 uint32_t a_cstimer = 0;
 uint32_t d_cstimer = 0;
 uint32_t w_cstimer = 0;
 uint32_t s_cstimer = 0;
 uint8_t perfect_input_count = 0;
 
 bool rapidfire = false;
 uint32_t rapidfire_timer = 0;
 
 enum layers {
     _LAYER0,
     _LAYER1,
     _LAYER2,
     _LAYER3,
 };
 
 const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 
     [_LAYER0] = LAYOUT_tkl_f13_ansi_tsangan(
         KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,     KC_PSCR, KC_SCRL, KC_PAUS,
         KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,
         KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,
         KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
         KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,
         KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                                      KC_RALT, MO(1),   KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT
     ),
 
     [_LAYER1] = LAYOUT_tkl_f13_ansi_tsangan(
         KC_SLEP, EE_CLR,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TO(2),      KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT,    KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_MPLY,
         KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,             KC_KB_VOLUME_UP,
         KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS,    KC_MPRV, KC_KB_VOLUME_DOWN, KC_MNXT
     ),
 
     [_LAYER2] = LAYOUT_tkl_f13_ansi_tsangan(
         KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,     KC_PSCR, TO(0),   RAPIDF,
         KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,
         KC_TAB,  KC_Q,    ASTF_W,  KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,
         KC_CAPS, ASTF_A,  ASTF_S,  ASTF_D,  KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
         KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,
         KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                                      KC_RALT, MO(1),   KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT
     ),
 
     [_LAYER3] = LAYOUT_tkl_f13_ansi_tsangan(
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
         KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,             KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS
     )
 };
 
 // Called every clock cycle of the keyboard
 void matrix_scan_user(void) {
     if (a_state == CS_DELAY && timer_elapsed32(a_cstimer) > SOCD_OVERLAP_DELAY) {
         register_code(KC_D);
         a_state = CS_COUNTERSTRAFE;
         a_cstimer = timer_read32();
     } else if (a_state == CS_COUNTERSTRAFE && timer_elapsed32(a_cstimer) > COUNTERSTRAFE_DELAY) {
         if (!d_down) {
             unregister_code(KC_D);
         }
         a_state = CS_NEUTRAL;
     }
 
     if (d_state == CS_DELAY && timer_elapsed32(d_cstimer) > SOCD_OVERLAP_DELAY) {
         register_code(KC_A);
         d_state = CS_COUNTERSTRAFE;
         d_cstimer = timer_read32();
     } else if (d_state == CS_COUNTERSTRAFE && timer_elapsed32(d_cstimer) > COUNTERSTRAFE_DELAY) {
         if (!a_down) {
             unregister_code(KC_A);
         }
         d_state = CS_NEUTRAL;
     }
 
     if (w_state == CS_DELAY && timer_elapsed32(w_cstimer) > SOCD_OVERLAP_DELAY) {
         register_code(KC_S);
         w_state = CS_COUNTERSTRAFE;
         w_cstimer = timer_read32();
     } else if (w_state == CS_COUNTERSTRAFE && timer_elapsed32(w_cstimer) > COUNTERSTRAFE_DELAY) {
         if (!s_down) {
             unregister_code(KC_S);
         }
         w_state = CS_NEUTRAL;
     }
 
     if (s_state == CS_DELAY && timer_elapsed32(s_cstimer) > SOCD_OVERLAP_DELAY) {
         register_code(KC_W);
         s_state = CS_COUNTERSTRAFE;
         s_cstimer = timer_read32();
     } else if (s_state == CS_COUNTERSTRAFE && timer_elapsed32(s_cstimer) > COUNTERSTRAFE_DELAY) {
         if (!w_down) {
             unregister_code(KC_W);
         }
         s_state = CS_NEUTRAL;
     }
 
     if (rapidfire && timer_elapsed32(rapidfire_timer) >= RAPIDFIRE_INTERVAL) {
         tap_code(KC_MS_BTN1);
         rapidfire_timer = timer_read32();
     }
 }
 
 // Event responder when key is pressed/released
 bool process_record_user(uint16_t keycode, keyrecord_t *record) {
     switch (keycode) {
         case ASTF_A:
             if (record->event.pressed) {
                 a_down = true;
                 if (a_state == CS_COUNTERSTRAFE) {
                     unregister_code(KC_D);
                 }
                 a_state = CS_HOLD;
                 register_code(KC_A);
             } else {
                 unregister_code(KC_A);
                 if (!d_down) {
                     if (perfect_input_count >= PERFECT_INPUTS_BEFORE_DELAY) {
                         a_state = CS_DELAY;
                     } else {
                         a_state = CS_COUNTERSTRAFE;
                         register_code(KC_D);
                     }
                     a_cstimer = timer_read32();
                     perfect_input_count = (perfect_input_count + 1) % (PERFECT_INPUTS_BEFORE_DELAY + 1);
                 }
                 a_down = false;
             }
             return false;
         case ASTF_D:
             if (record->event.pressed) {
                 d_down = true;
                 if (d_state == CS_COUNTERSTRAFE) {
                     unregister_code(KC_A);
                 }
                 d_state = CS_HOLD;
                 register_code(KC_D);
             } else {
                 unregister_code(KC_D);
                 if (!a_down) {
                     if (perfect_input_count >= PERFECT_INPUTS_BEFORE_DELAY) {
                         d_state = CS_DELAY;
                     } else {
                         d_state = CS_COUNTERSTRAFE;
                         register_code(KC_A);
                     }
                     d_cstimer = timer_read32();
                     perfect_input_count = (perfect_input_count + 1) % (PERFECT_INPUTS_BEFORE_DELAY + 1);
                 }
                 d_down = false;
             }
             return false;
         
         case ASTF_W:
             if (record->event.pressed) {
                 w_down = true;
                 if (w_state == CS_COUNTERSTRAFE) {
                     unregister_code(KC_S);
                 }
                 w_state = CS_HOLD;
                 register_code(KC_W);
             } else {
                 unregister_code(KC_W);
                 if (!s_down) {
                     if (perfect_input_count >= PERFECT_INPUTS_BEFORE_DELAY) {
                         w_state = CS_DELAY;
                     } else {
                         w_state = CS_COUNTERSTRAFE;
                         register_code(KC_S);
                     }
                     w_cstimer = timer_read32();
                     perfect_input_count = (perfect_input_count + 1) % (PERFECT_INPUTS_BEFORE_DELAY + 1);
                 }
                 w_down = false;
             }
             return false;
         case ASTF_S:
             if (record->event.pressed) {
                 s_down = true;
                 if (s_state == CS_COUNTERSTRAFE) {
                     unregister_code(KC_W);
                 }
                 s_state = CS_HOLD;
                 register_code(KC_S);
             } else {
                 unregister_code(KC_S);
                 if (!w_down) {
                     if (perfect_input_count >= PERFECT_INPUTS_BEFORE_DELAY) {
                         s_state = CS_DELAY;
                     } else {
                         s_state = CS_COUNTERSTRAFE;
                         register_code(KC_W);
                     }
                     s_cstimer = timer_read32();
                     perfect_input_count = (perfect_input_count + 1) % (PERFECT_INPUTS_BEFORE_DELAY + 1);
                 }
                 s_down = false;
             }
             return false;
         case RAPIDF:
             if (record->event.pressed) {
                 rapidfire = !rapidfire;
                 rapidfire_timer = timer_read32();
             }
             break;
     }
     return true;
 }
