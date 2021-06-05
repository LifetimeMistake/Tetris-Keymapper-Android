#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "event_injector.c"
#include "util.c"
#include <math.h>
#include <sys/time.h>
#include "list.c"

int last_reported_finger_id = -1;
int fingers_down_count = 0;
struct list_node* finger_ids_list = NULL;
unsigned int next_touch_id = 0;

unsigned int reserve_finger_id()
{
    fingers_down_count++;
    if(finger_ids_list == NULL)
    {
        finger_ids_list = list_create(0);
        return 0;
    }

    int next_free_id = 0;
    while (list_contains(finger_ids_list, next_free_id))
    {
        next_free_id++;
    }

    finger_ids_list = list_add(finger_ids_list, next_free_id);
    return next_free_id;
}

bool release_finger_id(unsigned int finger_id)
{
    if(finger_ids_list == NULL)
    {
        printf("warn: what the fuck\n");
        return false;
    }

    finger_ids_list = list_remove_values(finger_ids_list, finger_id);
    fingers_down_count--;
    return true;
}

bool inject_finger_switch(int event_fd, unsigned int finger_id)
{
    last_reported_finger_id = finger_id;
    return inject_device_event(event_fd, 3, 47, finger_id);
}

bool inject_location_update(int event_fd, unsigned int finger_id, unsigned int location_x, unsigned int location_y)
{
    inject_finger_switch(event_fd, finger_id);
    inject_device_event(event_fd, 3, 53, location_x);
    inject_device_event(event_fd, 3, 54, location_y);
    inject_device_event(event_fd, 0, 0, 0);

    return true;
}

unsigned int inject_finger_down(int event_fd, unsigned int location_x, unsigned int location_y)
{
    unsigned int finger_id = reserve_finger_id();

    inject_finger_switch(event_fd, finger_id);
    inject_device_event(event_fd, 3, 57, next_touch_id++);
    inject_device_event(event_fd, 3, 58, 255);

    if(fingers_down_count == 1)
    {
        inject_device_event(event_fd, 1, 330, 1);
    }

    inject_location_update(event_fd, finger_id, location_x, location_y);

    return finger_id;
}

bool inject_finger_up(int event_fd, unsigned int finger_id)
{
    inject_finger_switch(event_fd, finger_id);
    inject_device_event(event_fd, 3, 57, -1);

    if(fingers_down_count == 1)
    {
        inject_device_event(event_fd, 1, 330, 0);
    }
    inject_device_event(event_fd, 0, 0, 0);

    release_finger_id(finger_id);

    return true;
}

bool inject_tap_gesture(int event_fd, unsigned int location_x, unsigned int location_y)
{
    unsigned int finger_id = inject_finger_down(event_fd, location_x, location_y);
    inject_finger_up(event_fd, finger_id);

    return true;
}

bool inject_swipe_gesture(int event_fd, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y, unsigned int duration)
{
    unsigned int finger_id = inject_finger_down(event_fd, start_x, start_y);

    usleep((duration/2*1000));

    inject_location_update(event_fd, finger_id, end_x, end_y);

    usleep((duration/2*1000));

    inject_finger_up(event_fd, finger_id);

    return true;
}

bool inject_swipe_gesture_full(int event_fd, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y, unsigned int duration)
{
    unsigned int finger_id = inject_finger_down(event_fd, start_x, start_y);

    struct timeval *start = malloc(sizeof(struct timeval));
    struct timeval *stop = malloc(sizeof(struct timeval));
    gettimeofday(start, NULL);
    for(int i = 0; i<duration*1000; i = (stop->tv_usec - start->tv_usec))
    {
        gettimeofday(stop, NULL);
        float step = (float)i/(float)(duration*1000);
        unsigned int current_x = (unsigned int)lerp((float)start_x, (float)end_x, step);
        unsigned int current_y = (unsigned int)lerp((float)start_y, (float)end_y, step);
        inject_location_update(event_fd, finger_id, current_x, current_y);
    }

    free(start);
    free(stop);

    inject_finger_up(event_fd, finger_id);
    return true;
}