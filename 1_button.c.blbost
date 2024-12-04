#define BUTT1 PB5 // first button

#include "timer.h"
#include <avr/interrupt.h> // Interrupts standard C library for AVR-GCC
#include "gpio.h"
#include "uart.h"

#ifndef F_CPU
#define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif
#define debounce_delay 12


volatile uint8_t button1_pressed_cycles = 0;
volatile uint8_t button1_active = 0;
volatile uint8_t value_was_sent = 0;

int main(void)
{

  uart_init(UART_BAUD_SELECT(115200, F_CPU));

  TIM0_ovf_1ms();
  TIM0_ovf_enable();
  sei();
  GPIO_mode_input_pullup(&DDRB, BUTT1);

  while (1)
  {

    if (GPIO_read(&PINB, BUTT1) == 0)
    {
      button1_active = 1;
    }
    else
    {
      button1_active = 0;
    }

    if (button1_pressed_cycles >= debounce_delay && value_was_sent == 0) //kdyz je tlacitko zmackle delsi dobu nez debounce to jeste to nic neposlalo 
    {
      uart_putc('1');             // Posli 1
      value_was_sent = 1;         //hodnota byla odeslana =1
      button1_pressed_cycles = 0; // vynulovat čítač
    }
  }
}

ISR(TIMER0_OVF_vect)
{
  if (button1_active == 1)
  {
    button1_pressed_cycles++;
  }
  else
  {
    button1_pressed_cycles = 0; // vynulovat čítač
    value_was_sent = 0;
  }
}

// new comment