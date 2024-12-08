/**
 * @file main.c
 * @brief Main program for debouncing pressed buttons and playing tones or playing songs.
 *
 * This program detects button presses using GPIO pins and triggers
 * either UART transmission or song playback based on the mode.
 * Debouncing is implemented using a timer interrupt.
 */

#ifndef F_CPU
#define F_CPU 16000000 /**< CPU frequency in Hz required for UART_BAUD_SELECT */
#endif

#include "timer.h"
#include <avr/interrupt.h>
#include "gpio.h"
#include "uart.h"
#include "memory_song.h"

#define DEBOUNCE_DELAY 12 /**< Number of cycles to wait for button debouncing */

/** @brief Button pin definitions for the buttons connected to GPIO. */
#define BUTT1 PB5 /**< Button 1 pin (Port B, Pin 5) */
#define BUTT2 PB4 /**< Button 2 pin (Port B, Pin 4) */
#define BUTT3 PB3 /**< Button 3 pin (Port B, Pin 3) */
#define BUTT4 PB2 /**< Button 4 pin (Port B, Pin 2) */
#define BUTT5 PB1 /**< Button 5 pin (Port B, Pin 1) */
#define BUTT6 PB0 /**< Button 6 pin (Port B, Pin 0) */
#define BUTT7 PD7 /**< Button 7 pin (Port D, Pin 7) */
#define BUTT8 PD6 /**< Button 8 pin (Port D, Pin 6) */
#define MODE PD5  /**< Mode switch pin (Port D, Pin 5) */

/**
 * @brief Array of button pins for easy iteration.
 */
const uint8_t button_pins[] = {BUTT1, BUTT2, BUTT3, BUTT4, BUTT5, BUTT6, BUTT7, BUTT8};

/**
 * @brief Array of button ports for corresponding pins.
 */
volatile uint8_t *button_ports[] = {&PINB, &PINB, &PINB, &PINB, &PINB, &PINB, &PIND, &PIND};

/**
 * @brief Reset of counter for debouncing each button.
 */
volatile uint8_t button_pressed_cycles[8] = {0};

/**
 * @brief Reset of active buttons. Every button is unactive now.
 */
volatile uint8_t button_active[8] = {0};

/**
 * @brief Reset of variable that detect, that value vas sent to uart.
 */
volatile uint8_t value_was_sent[8] = {0};

/**
 * @brief Configures button pins as input with pull-up resistors.
 */
void setup_buttons(void)
{
    GPIO_mode_input_pullup(&DDRB, BUTT1);
    GPIO_mode_input_pullup(&DDRB, BUTT2);
    GPIO_mode_input_pullup(&DDRB, BUTT3);
    GPIO_mode_input_pullup(&DDRB, BUTT4);
    GPIO_mode_input_pullup(&DDRB, BUTT5);
    GPIO_mode_input_pullup(&DDRB, BUTT6);
    GPIO_mode_input_pullup(&DDRD, BUTT7); 
    GPIO_mode_input_pullup(&DDRD, BUTT8); 
    GPIO_mode_input_pullup(&DDRD, MODE);  /* Mode switch */
}

/**
 * @brief Main function of the program.
 *
 * Initializes UART, timer, and button configuration. Enters an infinite loop to
 * check button states and either transmit data via UART to play single tones or play a song from memory.
 */
int main(void)
{
    uart_init(UART_BAUD_SELECT(115200, F_CPU));
    TIM0_ovf_1ms();    /**< Initialize timer for 1ms overflow */
    TIM0_ovf_enable(); /**< Enable timer overflow interrupt */
    sei();             /**< Enable global interrupts */

    setup_buttons(); /**< Configure button pins */

    while (1) 
    {
        for (uint8_t i = 0; i < 8; i++) /**< In loop check all the buttons */
        {
            if (GPIO_read(button_ports[i], button_pins[i]) == 0) /**< When "i" button is pressed, set button_active[i] to 1 */
            {
                button_active[i] = 1;
            }
            else
            {
                button_active[i] = 0;
            }

            if (button_pressed_cycles[i] >= DEBOUNCE_DELAY && value_was_sent[i] == 0) /**< DEBOUNCE-When button was marked as pressed at least "DEBOUNCE_DELAY" times in a row and do not sent his value yet*/
            {
                if (GPIO_read(&PIND, MODE) == 0) /**< When MODE switch is on*/
                {
                    uart_putc('0' + i); /**< Send button index via UART */
                }
                else                            /**< When MODE switch is off*/
                {
                    play_song(i); /**< Play song corresponding to the pressed button */
                }
                value_was_sent[i] = 1; /**< Set value_was_sent to 1*/
                button_pressed_cycles[i] = 0; /**< Reset counter */
            }
        }
    }
}

/**
 * @brief Timer interrupt service routine.
 *
 * Called on Timer 0 overflow. Handles button debouncing by incrementing
 * counters and resetting states. Decrement value "currentDelay" by calling function decrementDelay()
 */
ISR(TIMER0_OVF_vect)
{
    if (decrementDelay())
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            if (button_active[i] == 1) /**< When button is still pressed, increment variable "button_pressed_cycles" which sums number of active value in a row. */
            {
                button_pressed_cycles[i]++;
            }
            else                        /**< Button is not pressed, reset everything */
            {
                button_pressed_cycles[i] = 0; /**<Reset counter */
                value_was_sent[i] = 0;  /**<Number was not sent to UART */
            }
        }
    }
}
