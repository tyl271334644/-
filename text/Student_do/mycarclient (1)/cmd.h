#ifndef CMD_H
#define CMD_H
#define CMD_SIZE 5

static const char CMD_ROLL_STOP[]     = {0xff, 0, 0, 0, 0xff};
static const char CMD_ROLL_FORWARD[]  = {0xff, 0, 1, 0, 0xff};
static const char CMD_ROLL_BACKWARD[] = {0xff, 0, 2, 0, 0xff};
static const char CMD_ROLL_LEFT[]     = {0xff, 0, 3, 0, 0xff};
static const char CMD_ROLL_RIGHT[]    = {0xff, 0, 4, 0, 0xff};

static const char CMD_NORMAL_MODE[]     = {0xff, 0x13, 0x00, 0x00, 0xff};
static const char CMD_TRAILING_MODE[]   = {0xff, 0x13, 0x01, 0x00, 0xff};
//static const char CMD_NORMAL_MODE[]   = {0xff, 0x13, 0x04, 0x00, 0xff};
#endif // CMD_H

