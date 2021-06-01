#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "buttons.h"
#include "gesture_injector.c"
#include "game_state.h"
#include "button_logic.c"
#include "dpad_logic.c"

bool debounce = true;
int DPAD_HAXIS_STATE = 0;
int GAME_STATE = GAME_STATE_MENU;

void process_button_event(int event_fd, input_event_t* event)
{
    if(event->event_type != 1) 
    {
        printf("warn: Button event handler cannot handle event type %d\n", event->event_type);
        return;
    }

    if(event->value != 0) 
    {
        return;
    }

    switch(event->event_code)
    {
        case BUTTON_A:
            GAME_STATE = process_button_A(event_fd, GAME_STATE);
            break;
        case BUTTON_B:
            GAME_STATE = process_button_B(event_fd, GAME_STATE);
            break;
        case BUTTON_START:
            GAME_STATE = process_button_START(event_fd, GAME_STATE);
            break;
        case BUTTON_LB:
            GAME_STATE = process_button_LB(event_fd, GAME_STATE);
            break;
        case BUTTON_RB:
            GAME_STATE = process_button_RB(event_fd, GAME_STATE);
            break;
        default:
            printf("warn: Unhandled button: %d\n", event->event_code);
            break;
    }
}

void process_dpad_event(int event_fd, input_event_t* event)
{
    if (event->event_type != 3) 
    {
        printf("warn: D-Pad event handler cannot handle event type %d\n", event->event_type);
        return;
    }

    switch(event->event_code)
    {
        case DPAD_VAXIS:
            GAME_STATE = process_dpad_vertical(event_fd, event->value, GAME_STATE);
            break;
        case DPAD_HAXIS:
            if(DPAD_HAXIS_STATE != event->value)
            {
                debounce = true;
            }
            else if(debounce)
            {
                debounce = false;
                break;
            }
            DPAD_HAXIS_STATE = event->value;
            GAME_STATE = process_dpad_horizontal(event_fd, event->value, GAME_STATE);
            break;
    }
}

void replay_last_event(int event_fd)
{
    if (DPAD_HAXIS_STATE == 0)
    {
        //printf("info: Nothing to do!\n");
        return;
    }

    input_event_t event;
    memset(&event, 0, sizeof(event));
    event.event_type = 3;
    event.event_code = DPAD_HAXIS;
    event.value = DPAD_HAXIS_STATE;
    process_dpad_event(event_fd, &event);
    //free(&event);
}

void process_new_event(int event_fd, input_event_t* event)
{
    switch(event->event_type)
    {
        case 1:
            process_button_event(event_fd, event);
            break;
        case 3:
            process_dpad_event(event_fd, event);
            break;
        default:
            printf("warn: unknown event type %d\n", event->event_type);
            break;
    }
}