#ifndef F_CPU
#define F_CPU 16000000 // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

#include "timer.h"
#include <avr/interrupt.h> // Interrupts standard C library for AVR-GCC
#include "gpio.h"
#include "uart.h"
#include "memory_song.h"

#define DEBOUNCE_DELAY 12

// Button pin definitions
#define BUTT1 PB5
#define BUTT2 PB4
#define BUTT3 PB3
#define BUTT4 PB2
#define BUTT5 PB1
#define BUTT6 PB0
#define BUTT7 PD7 // musi byt PDx, PB ma jen 6 portu
#define BUTT8 PD6 // musi byt PDx
#define MODE PD5

const uint8_t button_pins[] = {BUTT1, BUTT2, BUTT3, BUTT4, BUTT5, BUTT6, BUTT7, BUTT8};
volatile uint8_t *button_ports[] = {&PINB, &PINB, &PINB, &PINB, &PINB, &PINB, &PIND, &PIND};

volatile uint8_t button_pressed_cycles[8] = {0};
volatile uint8_t button_active[8] = {0};
volatile uint8_t value_was_sent[8] = {0};

void setup_buttons(void)
{
    // Configure buttons as input with pull-up resistors
    GPIO_mode_input_pullup(&DDRB, BUTT1);
    GPIO_mode_input_pullup(&DDRB, BUTT2);
    GPIO_mode_input_pullup(&DDRB, BUTT3);
    GPIO_mode_input_pullup(&DDRB, BUTT4);
    GPIO_mode_input_pullup(&DDRB, BUTT5);
    GPIO_mode_input_pullup(&DDRB, BUTT6);
    GPIO_mode_input_pullup(&DDRD, BUTT7); // DDRD
    GPIO_mode_input_pullup(&DDRD, BUTT8); // DDRD
    GPIO_mode_input_pullup(&DDRD, MODE);
}

int main(void)
{
    uart_init(UART_BAUD_SELECT(115200, F_CPU));
    TIM0_ovf_1ms();    // Initialize timer for 1ms overflow
    TIM0_ovf_enable(); // Enable timer overflow interrupt
    sei();             // Enable global interrupts

    setup_buttons(); // Configure button pins

    while (1)
    {
        for (uint8_t i = 0; i < 8; i++)
        {

            if (GPIO_read(button_ports[i], button_pins[i]) == 0)
            {
                button_active[i] = 1;
            }
            else
            {
                button_active[i] = 0;
            }

            if (button_pressed_cycles[i] >= DEBOUNCE_DELAY && value_was_sent[i] == 0)
            {
                if (GPIO_read(&PIND, MODE) == 0)
                {
                    uart_putc('0' + i);
                }
                else
                {
                    play_song(i);
                }
                value_was_sent[i] = 1;
                button_pressed_cycles[i] = 0;
            }
        }
    }
}

ISR(TIMER0_OVF_vect)
{
    if (decrementDelay())
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            if (button_active[i] == 1)
            {
                button_pressed_cycles[i]++;
            }
            else
            {
                button_pressed_cycles[i] = 0;
                value_was_sent[i] = 0;
            }
        }
    }
}

// another new comment