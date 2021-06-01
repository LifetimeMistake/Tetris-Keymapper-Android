#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game_state.h"
#include "gesture_injector.c"
#include "config.h"

int process_dpad_vertical(int event_fd, int value, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_PAUSED:
            if (value == -1)
            {
                inject_tap_gesture(event_fd, QUIT_GAME_ACTION);
                return GAME_STATE_MENU; 
            }
            if (value == 1)
            {
                inject_tap_gesture(event_fd, CONTINUE_GAME_ACTION);
                return GAME_STATE_PLAYING;
            }
            break;
        case GAME_STATE_PLAYING:
            if (value == -1)
            {
                inject_swipe_gesture(event_fd, HARD_DROP_ACTION, TETRIMINO_MOVE_TIME);
            }
            break;
    }

    return game_state;
}

int process_dpad_horizontal(int event_fd, int value, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_PAUSED:
            if (value == 1)
            {
                inject_tap_gesture(event_fd, NEW_GAME_ACTION);
                return GAME_STATE_PLAYING;
            }
            break;
        case GAME_STATE_PLAYING:
            if (value == -1)
            {
                inject_swipe_gesture(event_fd, MOVE_LEFT_ACTION, TETRIMINO_MOVE_TIME);
            }
            if (value == 1)
            {
                inject_swipe_gesture(event_fd, MOVE_RIGHT_ACTION, TETRIMINO_MOVE_TIME);
            }
            break;
    }

    return game_state;
}