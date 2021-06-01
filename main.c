#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <string.h>
#include "config.h"
#include "event.c"
#include "driver_logic.c"

int main()
{
    int event_count;
    struct pollfd fds[1];

    fds[0].fd = open(GAMEPAD_EVENT_PATH, O_RDONLY | O_NONBLOCK);
    if (fds[0].fd < 0)
    {
        printf("Could not open device '%s'\n", GAMEPAD_EVENT_PATH);
        return 1;
    }

    int event_fd = open_touchscreen_fd(TS_EVENT_PATH);
    if (event_fd < 0)
    {
        printf("Could not open device '%s'\n", TS_EVENT_PATH);
        return 1;
    }

    unsigned char input_data[GAMEPAD_EVENT_READ_SIZE];
    memset(input_data, 0, GAMEPAD_EVENT_READ_SIZE);

    fds[0].events = POLLIN;
    while(true)
    {
        event_count = poll(fds, 1, POLL_TIMEOUT_MS);
        if(event_count > 0)
        {
            if(!fds[0].revents)
            {
                printf("warn: Unexpected error: no events were available at fd[0]\n");
                continue;
            }

            ssize_t byte_count = read(fds[0].fd, input_data, GAMEPAD_EVENT_READ_SIZE);
            if (byte_count < 0)
            {
                printf("warn: Unexpected error: read failed\n");
                continue;
            }

            input_event_t* event = (input_event_t*)input_data;
            process_new_event(event_fd, event);
            memset(input_data, 0, GAMEPAD_EVENT_READ_SIZE);
        }

        if(event_count == 0)
        {
            //printf("info: Handling dpad without new events\n");
            replay_last_event(event_fd);
        }
    }

    close(fds[0].fd);
    close_touchscreen_fd(event_fd);
    return 0;
}