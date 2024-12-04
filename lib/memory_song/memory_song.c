#include "memory_song.h"
#include "uart.h"
#include <stdlib.h>

static volatile uint16_t currentDelay = 0;


struct Song
{
    uint8_t notes[20];
    uint16_t delays[20];
};

static volatile struct Song listOfSongs[] = {
    {.notes = {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0},
     .delays = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 0, 0, 0, 0, 0}},
    {.notes = {7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7},
     .delays = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 0, 0, 0, 0, 0}}};

void play_song(uint8_t nSong)
{

    if (nSong < (sizeof(listOfSongs) / sizeof(listOfSongs[0])))
    {
        for (uint8_t i = 0; i < 20; i++)
        {
            uart_putc('0' + listOfSongs[nSong].notes[i]);
            currentDelay = listOfSongs[nSong].delays[i];
            while (currentDelay != 0)
            {
            }
            
        }
    }
}

//
uint8_t decrementDelay()
{
    if (currentDelay > 0)
    {
        currentDelay--;
        
        return 0;
    }
    else
    {
        
        return 1;
    }
}
