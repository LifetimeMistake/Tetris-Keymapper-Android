#pragma once
#pragma pack(1)
typedef struct
{
    long time_seconds;
    long time_suseconds;
    unsigned short event_type;
    unsigned short event_code;
    int value;
} input_event_t;