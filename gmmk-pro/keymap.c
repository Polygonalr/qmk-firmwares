/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum custom_layers {
        _BASE,
        _FN1,
};

enum custom_keycodes {
    KC_MCR1 = SAFE_RANGE,
    KC_MCR2,
    KC_WINLCK,    //Toggles Win key on and off
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12       Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)       BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    [_BASE] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_FN1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_INS,          KC_MCR1,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PSCR, _______, _______, RESET,            KC_HOME,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          KC_END,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, KC_F13,
        _______,KC_WINLCK,_______,                            _______,                            _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT
    ),
};


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (IS_LAYER_ON(_FN1)) { // Encoder will scroll left or right if so if FN is held
        if (clockwise) {
            tap_code(KC_MS_WH_RIGHT);
        } else {
            tap_code(KC_MS_WH_LEFT);
        }
    } else {
        if (clockwise) { // Encoder will modify the volume 
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Light up left LEDs when caps lock is toggled on
    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t i = 0; i < 23; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(i, 0xFF, 0x00, 0x00);
        }
        RGB_MATRIX_INDICATOR_SET_COLOR(67, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(70, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(73, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(76, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(80, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(83, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(87, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(91, 0xFF, 0x00, 0x00);
    }
    // Light up right LEDs when FN is held
    if (get_highest_layer(layer_state) == 1) {
        for (uint8_t i = 61; i < 67; i++) {
            RGB_MATRIX_INDICATOR_SET_COLOR(i, 0, 0xFF, 56);
        }
        RGB_MATRIX_INDICATOR_SET_COLOR(68, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(69, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(71, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(72, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(74, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(75, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(77, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(78, 0, 0xFF, 56);  
        RGB_MATRIX_INDICATOR_SET_COLOR(79, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(81, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(82, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(84, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(85, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(86, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(88, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(89, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(90, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(95, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(92, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(93, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(94, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(96, 0, 0xFF, 56);
        RGB_MATRIX_INDICATOR_SET_COLOR(97, 0, 0xFF, 56);
    }
    return true;
}
