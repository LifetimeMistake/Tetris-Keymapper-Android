#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "event.c"

int open_touchscreen_fd(char* event_path)
{
    int fd = open(event_path, O_WRONLY);
    if (fd < 0)
    {
        return -1;
    }

    return fd;
}

bool close_touchscreen_fd(int touchscreen_fd)
{
    return (close(touchscreen_fd) == 0);
}

bool inject_device_event(int event_fd, unsigned short type, unsigned short code, int value)
{
    input_event_t event;
    memset(&event, 0, sizeof(event));

    event.event_type = type;
    event.event_code = code;
    event.value = value;

    ssize_t res = write(event_fd, &event, sizeof(event));
    return res != -1;
}