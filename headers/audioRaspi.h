/* 
 * File:   audioRaspi.h
 * Author: magali
 *
 * Created on February 7, 2021, 9:20 PM
 */

#ifndef AUDIORASPI_H
#define AUDIORASPI_H

#include <SDL2/SDL.h>
#include "libaudio.h"

#define S2WAIT 180

enum status {
    NO_INIT,
    READY,
    STOPPED,
    PLAYING,
    PAUSED,
    FINISHED
};

void debug_status();

int play_sound(void);
int pause_sound(void);
int end_play(void);
int player_status(void);
int init_sound(void);
int set_file_to_play(const char *music_path);
int stop_sound(void);

#endif






