#pragma once
// General driver settings
#define TETRIMINO_MOVE_OFFSET 90
#define TETRIMINO_MOVE_TIME 50
#define GAMEPAD_EVENT_PATH "/dev/input/event11"

// Dangerous driver settings (don't touch if you don't know what you're doing)
#define POLL_TIMEOUT_MS 50
#define GAMEPAD_EVENT_READ_SIZE 48
#define TS_EVENT_PATH "/dev/input/event3"

// Back to safety
#define PAUSE_GAME_ACTION 950, 2275
#define CONTINUE_GAME_ACTION 550, 1475
#define HOLD_BLOCK_ACTION 105, 750
#define ROTATE_LEFT_ACTION 300, 1250
#define ROTATE_RIGHT_ACTION 800, 1250
#define QUIT_GAME_ACTION 550, 1185
#define NEW_GAME_ACTION 550, 1325

#define MOVE_LEFT_ACTION 550, 1185, 550 - TETRIMINO_MOVE_OFFSET, 1185
#define MOVE_RIGHT_ACTION 550, 1185, 550 + TETRIMINO_MOVE_OFFSET, 1185
#define HARD_DROP_ACTION 550, 1185, 550, 1485