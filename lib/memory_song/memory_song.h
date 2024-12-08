#ifndef MEMORY_SONG_H
#define MEMORY_SONG_H

/* 
 * Documentation of funciton used for playing tones and melodies from memory via Arduino Uno.
 * (c) 2019-2024 Ondrej Vlcek
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

/**
 * @file 
 * @defgroup memory_song Memory song <memory_song.h>
 * @code #include <memory_song.h> @endcode
 *
 * @brief Playing tones on electronic xylophone for AVR-GCC.
 *
 * 
 *
 * @note Based on AVR Libc Reference Manual.
 * @copyright (c) 2019-2024 Ondrej Vlcek, MIT license
 * @{
 */

#include <stdint.h>

/**
 * @brief Main function of the program.
 *
 * Initializes UART, timer, and button configuration. Enters an infinite loop to
 * check button states and either transmit data via UART to play single tones or play a song from memory.
 * @note Function is in forever loop and waits until any button is pressed
 *
 */
int main(void);

/**
 * @brief Timer interrupt service routine.
 *
 * Called on Timer 0 overflow. Handles button debouncing by incrementing
 * counters and resetting states. Decrement value "currentDelay" by calling function decrementDelay()
 */
ISR(TIMER0_OVF_vect);

/**
 * @brief Plays a song from memory based on its index.
 * 
 * Sends each note of the song via UART and waits for the specified delay
 * before proceeding to the next note. The song data is stored in
 * predefined list in memory.
 *
 * @param nSong Index of the song to play (first song has index 0).
 */
void play_song(uint8_t nSong);

/**
 * @brief Decrements the delay counter for the current note.
 * 
 * This function is intended to be called by a timer interrupt to manage note
 * delays. The delay is decreased by 1 each time the function is called.
 *
 * @return 1 if the delay counter has reached zero, 0 otherwise.
 */
uint8_t decrementDelay();

#endif 
