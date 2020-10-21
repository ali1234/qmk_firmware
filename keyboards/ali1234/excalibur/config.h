/*
Copyright 2017 Ruari Armstrong <ukkeyboards@gmail.com>

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

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xF055 // "UK"
#define PRODUCT_ID      0x0001 // 78
#define DEVICE_VER      0x0002
#define MANUFACTURER    Drevo
#define PRODUCT         Excalibur
#define DESCRIPTION     QMK keyboard firmware for Drevo Excalibur

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 17

/* key matrix pins */
#define MATRIX_ROW_PINS { A0, A1, A2, A3, A4, A5 }
#define MATRIX_COL_PINS { E1, F7, F6, F5, F4, F3, F2, F1, F0, E6, E7, B0, B1, E0, D4, D5, D7 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

#define DRIVER_LED_TOTAL       (85)
#define LED_DRIVER_LED_COUNT       (85)

#define RGB_MATRIX_CENTER { 64, 20 }

#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_TYPING_HEATMAP

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

#endif
