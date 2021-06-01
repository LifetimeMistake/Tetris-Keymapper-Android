#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game_state.h"
#include "gesture_injector.c"
#include "config.h"

int process_button_A(int event_fd, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_PLAYING:
            inject_tap_gesture(event_fd, ROTATE_LEFT_ACTION);
            break;
    }

    return game_state;
}

int process_button_B(int event_fd, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_PAUSED:
            inject_tap_gesture(event_fd, CONTINUE_GAME_ACTION);
            return GAME_STATE_PLAYING;
        case GAME_STATE_PLAYING:
            inject_tap_gesture(event_fd, ROTATE_RIGHT_ACTION);
            break;
    }

    return game_state;
}

int process_button_START(int event_fd, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_MENU:
            return GAME_STATE_PLAYING;
        case GAME_STATE_PAUSED:
            inject_tap_gesture(event_fd, CONTINUE_GAME_ACTION);
            return GAME_STATE_PLAYING;
        case GAME_STATE_PLAYING:
            inject_tap_gesture(event_fd, PAUSE_GAME_ACTION);
            return GAME_STATE_PAUSED;
    }

    return game_state;
}

int process_button_LB(int event_fd, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_PLAYING:
            inject_tap_gesture(event_fd, HOLD_BLOCK_ACTION);
            break;
    }

    return game_state;
}

int process_button_RB(int event_fd, int game_state)
{
    switch(game_state)
    {
        case GAME_STATE_PLAYING:
            inject_tap_gesture(event_fd, HOLD_BLOCK_ACTION);
            break;
    }

    return game_state;
}