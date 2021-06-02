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

typedef struct
{
    long seconds;
    long microseconds;
} timeval_t;

bool inject_location_update(int event_fd, unsigned int location_x, unsigned int location_y)
{
    inject_device_event(event_fd, 3, 53, location_x);
    inject_device_event(event_fd, 3, 54, location_y);
    inject_device_event(event_fd, 0, 0, 0);

    return true;
}

bool inject_finger_down(int event_fd, unsigned int location_x, unsigned int location_y)
{
    inject_device_event(event_fd, 3, 57, random_int(10000));
    inject_device_event(event_fd, 3, 58, 255);
    inject_device_event(event_fd, 1, 330, 1);
    inject_location_update(event_fd, location_x, location_y);

    return true;
}

bool inject_finger_up(int event_fd)
{
    inject_device_event(event_fd, 3, 57, -1);
    inject_device_event(event_fd, 1, 330, 0);
    inject_device_event(event_fd, 0, 0, 0);

    return true;
}

bool inject_tap_gesture(int event_fd, unsigned int location_x, unsigned int location_y)
{
    //printf("injecting tap event at %d %d\n", location_x, location_y);
    inject_finger_down(event_fd, location_x, location_y);
    inject_finger_up(event_fd);

    return true;
}

bool inject_swipe_gesture(int event_fd, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y, unsigned int duration)
{
    //printf("injecting swipe event from %d %d to %d %d with duration %d ms\n", start_x, start_x, end_x, end_y, duration);
    /*inject_device_event(event_fd, 3, 57, random_int(10000));
    inject_device_event(event_fd, 3, 58, 255);
    inject_device_event(event_fd, 3, 53, start_x);
    inject_device_event(event_fd, 3, 54, start_y);
    inject_device_event(event_fd, 1, 330, 1);
    inject_device_event(event_fd, 0, 0, 0);*/
    inject_finger_down(event_fd, start_x, start_y);

    /*timeval_t start, stop;
    gettimeofday(&start, NULL);
    for(int i = 0; i<duration*1000; i = (stop.microseconds - start.microseconds))
    {
        gettimeofday(&stop, NULL);
        //printf("current i = %d", i);
        float step = (float)i/(float)(duration*1000);
        unsigned int current_x = (unsigned int)lerp((float)start_x, (float)end_x, step);
        unsigned int current_y = (unsigned int)lerp((float)start_y, (float)end_y, step);
        //printf("swipe event at %d %d %f\n", current_x, current_y, step);
        inject_device_event(event_fd, 3, 53, current_x);
        inject_device_event(event_fd, 3, 54, current_y);
        inject_device_event(event_fd, 0, 0, 0);
    }*/

    usleep((duration/2*1000));

    inject_location_update(event_fd, end_x, end_y);

    usleep((duration/2*1000));

    inject_finger_up(event_fd);

    return true;
}