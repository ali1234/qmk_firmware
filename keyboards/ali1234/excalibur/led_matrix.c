#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "quantum.h"
#include "ledmatrix.h"
#include "led.h"

#define __ (0)

static const uint8_t PROGMEM led_map[MATRIX_ROWS][MATRIX_COLS] = {
  {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, __, 14, __, 15 },
  { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, __, 31, 32 },
  { 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 16, 49, 33 },
  { 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, __, 63, __, __, __ },
  { 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, __, __, __, 48, __ },
  { 77, 78, 79, 80, 81, 82, __, __, 83, __, __, __, __, __, 84, 76, 50 } 
};


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


void set_value(int index, uint8_t value) {
    led_state[index] = value;
}

void set_value_all(uint8_t value) {
    for (int n=0; n<LED_DRIVER_LED_COUNT; n++) {
        led_state[n] = value;
    }
}




void init(void) {

    // clear LED state to all fully on
    set_value_all(255);

    // enable timer 1 and 3 fast pwm
    TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<COM1C1) | (1<<WGM10);
    TCCR3A = (1<<COM3A1) | (1<<COM3B1) | (1<<COM3C1) | (1<<WGM30);
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


const led_matrix_driver_t led_matrix_driver = {
    .init          = init,
    .flush         = flush,
    .set_value     = set_value,
    .set_value_all = set_value_all,
};


ISR (TIMER2_OVF_vect)
{
    static uint8_t col_index = 0;
    static uint16_t x = 0;
    x+=8;

    setPinInputHigh(col_pins[col_index]);

    col_index = (col_index + 1) % MATRIX_COLS;

    #define BRIGHTNESS(row) pgm_read_byte(&gamma8[led_state[pgm_read_byte(&led_map[row][col_index])]]);

    // load up the PWM values
    OCR3A = BRIGHTNESS(0)
    OCR3B = BRIGHTNESS(1)
    OCR3C = BRIGHTNESS(2)
    OCR1C = BRIGHTNESS(3)
    OCR1B = BRIGHTNESS(4)
    OCR1A = BRIGHTNESS(5)

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
            //led_state[pgm_read_byte(&led_map[row_index][col_index])] = 255;
        } else {
            // Pin HI, clear col bit
            raw_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << col_index);
        }
    }
}

