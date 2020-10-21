#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "quantum.h"
#include "rgb_matrix.h"
#include "led.h"

#define __ (NO_LED)


led_config_t g_led_config = {
    {
        { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, __, 13, __, 14},
        {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, __, 30, 31},
        {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 15, 48, 32},
        {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, __, 62, __, __, __},
        {63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, __, __, __, 47, __},
        {76, 77, 78, 79, 80, 81, __, __, 82, __, __, __, __, __, 83, 75, 49}
    }, {
        {  4,   0}, { 12,   0}, { 20,   0}, { 28,   0}, { 36,   0}, { 44,   0}, { 52,   0},
        { 60,   0}, { 68,   0}, { 76,   0}, { 84,   0}, { 92,   0}, {100,   0}, {108,   0},
        {116,   0}, {124,   0}, {  4,   8}, { 12,   8}, { 20,   8}, { 28,   8}, { 36,   8},
        { 44,   8}, { 52,   8}, { 60,   8}, { 68,   8}, { 76,   8}, { 84,   8}, { 92,   8},
        {100,   8}, {112,   8}, {124,   8}, {124,  16}, {114,  24}, {  6,  16}, { 16,  16},
        { 24,  16}, { 32,  16}, { 40,  16}, { 48,  16}, { 56,  16}, { 64,  16}, { 72,  16},
        { 80,  16}, { 88,  16}, { 96,  16}, {104,  16}, {114,  16}, {110,  32}, {118,  32},
        {124,  40}, {  7,  24}, { 18,  24}, { 26,  24}, { 34,  24}, { 42,  24}, { 50,  24},
        { 58,  24}, { 66,  24}, { 74,  24}, { 82,  24}, { 90,  24}, { 98,  24}, {106,  24},
        {  9,  32}, { 22,  32}, { 30,  32}, { 38,  32}, { 46,  32}, { 54,  32}, { 62,  32},
        { 70,  32}, { 78,  32}, { 86,  32}, { 94,  32}, {102,  32}, {116,  40}, {  5,  40},
        { 15,  40}, { 25,  40}, { 55,  40}, { 84,  40}, { 92,  40}, {100,  40}, {108,  40}
    }, {
        4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 8,
        8, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4,
        4, 8, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4,
        1, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 1,
        1, 1, 4, 1, 1, 1, 4
    }
};

/*
static const uint8_t PROGMEM led_map[MATRIX_ROWS][MATRIX_COLS] = {
  {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, __, 14, __, 15 },
  { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, __, 31, 32 },
  { 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 16, 49, 33 },
  { 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, __, 63, __, __, __ },
  { 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, __, __, __, 48, __ },
  { 77, 78, 79, 80, 81, 82, __, __, 83, __, __, __, __, __, 84, 76, 50 } 
};
*/

static const uint8_t PROGMEM gamma8[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 253, 253,
    253, 252, 252, 252, 252, 252, 252, 252, 251, 251, 251, 251, 251, 250, 250, 250,
    250, 249, 249, 249, 249, 248, 248, 248, 248, 247, 247, 247, 246, 246, 246, 245,
    245, 245, 244, 244, 244, 243, 243, 242, 242, 242, 241, 241, 240, 240, 239, 239,
    238, 238, 237, 237, 236, 236, 235, 235, 234, 234, 233, 233, 232, 231, 231, 230,
    230, 229, 228, 228, 227, 226, 226, 225, 224, 223, 223, 222, 221, 220, 220, 219,
    218, 217, 216, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 205,
    204, 203, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 189, 188, 187,
    186, 185, 183, 182, 181, 180, 178, 177, 176, 174, 173, 172, 170, 169, 168, 166,
    165, 163, 162, 160, 159, 157, 156, 154, 153, 151, 150, 148, 146, 145, 143, 141,
    140, 138, 136, 135, 133, 131, 129, 128, 126, 124, 122, 120, 118, 117, 115, 113,
    111, 109, 107, 105, 103, 101,  99,  97,  95,  93,  91,  88,  86,  84,  82,  80,
     78,  75,  73,  71,  69,  66,  64,  62,  59,  57,  55,  52,  50,  47,  45,  42,
     40,  37,  35,  32,  30,  27,  24,  22,  19,  16,  14,  11,   8,   6,   3,   0
};
#define GAMMA(v) pgm_read_byte(&gamma8[v])

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

extern volatile matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values

volatile uint8_t led_state[LED_DRIVER_LED_COUNT];
volatile uint8_t backlight_brightness = 40;


/*
void backlight_init_ports(void) {}


void backlight_set(uint8_t level) {
    backlight_brightness = level;
}


void backlight_task(void) {
    for (int n=0; n<LED_DRIVER_LED_COUNT; n++) {
        switch (n) {
            case 51:
                break;
            default:
                if (led_state[n] > backlight_brightness)
                    led_state[n]--;
                else if (led_state[n] < backlight_brightness)
                    led_state[n]++;
        }
    }
}
*/


static void set_color(int index, uint8_t r, uint8_t g, uint8_t b) {
    led_state[index] = GAMMA(r);
}


static void set_color_all(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
        led_state[i] = GAMMA(r);
    }
}


/*
static void set_color_all_f(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
        led_state[i] = GAMMA(r);
    }
}
*/


void init(void) {

    // clear LED state to all fully on
    set_color_all(0, 0, 0);

    // enable timer 1 and 3 fast pwm
    TCCR1A = (1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<COM1C1) | (1<<COM1C0) | (1<<WGM10);
    TCCR3A = (1<<COM3A1) | (1<<COM3A0) | (1<<COM3B1) | (1<<COM3B0) | (1<<COM3C1) | (1<<COM3C0) | (1<<WGM30);
    TCCR1B = (1<<WGM12) | (1<<CS10);
    TCCR3B = (1<<WGM32) | (1<<CS30);

    // set all PWM to fully off
    OCR3A = 255; OCR3B = 255; OCR3C = 255; OCR1C = 255; OCR1B = 255; OCR1A = 255;

    // set GPIO to output
    DDRB |= 0xe0;
    DDRC |= 0x70;

    // enable timer 2 interrupt
    TCCR2A = 0x00;
    TCCR2B = (0<<CS22) | (1<<CS21) | (1<<CS20);
    TIMSK2 = (1 << TOIE2);

}

static void flush(void) {}


const rgb_matrix_driver_t rgb_matrix_driver = {
    .init          = init,
    .flush         = flush,
    .set_color     = set_color,
    .set_color_all = set_color_all,
};


ISR (TIMER2_OVF_vect)
{
    static uint8_t col_index = 0;
    static uint16_t x = 0;
    x+=8;

    setPinInputHigh(col_pins[col_index]);

    col_index = (col_index + 1) % MATRIX_COLS;

#define GETPWM(row) (led_state[g_led_config.matrix_co[row][col_index]])

    OCR3A = GETPWM(0);
    OCR3B = GETPWM(1);
    OCR3C = GETPWM(2);
    OCR1C = GETPWM(3);
    OCR1B = GETPWM(4);
    OCR1A = GETPWM(5);

    // set counters to TOP so that OCR registers are updated immediately
    TCNT1 = 255;
    TCNT3 = 255;

    writePinLow(col_pins[col_index]);
    setPinOutput(col_pins[col_index]);

    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Check row pin state
        if (readPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            raw_matrix[row_index] |= (MATRIX_ROW_SHIFTER << col_index);
        } else {
            // Pin HI, clear col bit
            raw_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << col_index);
        }
    }
}

