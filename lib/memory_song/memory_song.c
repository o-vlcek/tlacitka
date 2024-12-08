#include "memory_song.h"
#include "uart.h"
#include <stdlib.h>

/* 
 *  Stores the current delay for note playback. 
 * Updated and decremented by the timer interrupt service routine.
 */
static volatile uint16_t currentDelay = 0;

/*
 *  Song
 * Represents a single song with an array of notes and delays.
 * Every song is defined 20 tones and 20 delays after every tone.
 */
struct Song
{
    uint8_t notes[20];  /* Array of notes (0-7) in the song. */
    uint16_t delays[20]; /* Array of delays (in ms) corresponding to each note. */
};

/*
 * List of predefined songs stored in memory.
 */
static volatile struct Song listOfSongs[] = {
// Jingle Bells
.notes = {2, 2, 2, 2, 2, 2, 4, 1, 2, 3, 2, 2, 2, 2, 2, 2, 4, 1, 2, 4},
.delays = {500, 500, 1000, 500, 500, 1000, 1000, 500, 500, 1000, 500, 500, 1000, 500, 500, 1000, 1000, 500, 500, 1500},

// We Wish You a Merry Christmas
.notes = {0, 3, 3, 4, 3, 2, 1, 1, 1, 4, 4, 5, 4, 3, 2, 0, 3, 2, 3, 1},
.delays = {500, 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 1000},

// Twinkle Twinkle Little Star
.notes = {4, 4, 5, 5, 4, 3, 3, 2, 2, 1, 1, 0, 4, 4, 5, 5, 4, 3, 2, 1},
.delays = {500, 500, 500, 1000, 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 1000, 500, 500, 500, 500, 1000},

// Happy Birthday
.notes = {4, 4, 5, 4, 6, 4, 4, 5, 4, 1, 4, 4, 5, 4, 1, 0, 4, 2, 5, 3},
.delays = {500, 500, 1000, 500, 500, 1000, 500, 500, 1000, 500, 500, 1000, 500, 500, 1000, 500, 500, 500, 1000, 1000},

// Ode to Joy
.notes = {2, 2, 3, 4, 4, 3, 2, 1, 0, 1, 2, 2, 1, 1, 0, 2, 2, 3, 4, 3},
.delays = {500, 500, 500, 1000, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 1000, 500},

// Silent Night
.notes = {1, 2, 3, 1, 2, 3, 4, 3, 2, 1, 1, 3, 3, 4, 3, 2, 1, 0, 3, 3},
.delays = {500, 500, 1000, 500, 500, 1000, 500, 500, 500, 1000, 500, 500, 500, 1000, 500, 500, 500, 1000, 500, 1000},

// Jingle Bells Rock
.notes = {2, 2, 4, 4, 5, 5, 4, 4, 5, 3, 2, 4, 3, 5, 4, 3, 2, 4, 2, 1},
.delays = {500, 500, 500, 500, 500, 500, 1000, 500, 500, 1000, 500, 500, 500, 1000, 500, 500, 500, 1000, 500, 1000},

// Let It Snow
.notes = {4, 4, 5, 5, 6, 6, 5, 4, 4, 5, 5, 6, 6, 5, 4, 5, 5, 6, 6, 4},
.delays = {500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 1000}


};

/*
 * Plays a song based on the given index.
 * 
 * Sends each note of the song via UART and waits for the specified delay
 * before sending next note.
 * 
 * nSong is index value of the song to play.
 */
void play_song(uint8_t nSong)
{
    if (nSong < (sizeof(listOfSongs) / sizeof(listOfSongs[0]))) /* When song is in list */
    {
        for (uint8_t i = 0; i < 20; i++) /* For every tone in song*/
        {
            uart_putc('0' + listOfSongs[nSong].notes[i]); /* Send the note via UART */
            currentDelay = listOfSongs[nSong].delays[i]; /* Set the delay for the note */

            while (currentDelay != 0) /* Wait until the delay is decremented to zero */
            {
            }
        }
    }
}

/*
 * Decrements the current delay value.
 * 
 * This function is called from the timer interrupt routine to manage delays.
 * 
 * Returns 1 if the delay has reached zero, 0 otherwise.
 */
uint8_t decrementDelay()
{
    if (currentDelay > 0)
    {
        currentDelay--; /* Decrease delay by 1 */
        return 0;
    }
    else
    {
        return 1; /* Delay has reached zero */
    }
}
