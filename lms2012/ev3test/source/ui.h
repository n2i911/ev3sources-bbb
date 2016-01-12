#ifndef UI_H_
#define UI_H_

enum UiButPins
{
    BUT0,     // UP
    BUT1,     // ENTER
    BUT2,     // DOWN
    BUT3,     // RIGHT
    BUT4,     // LEFT
    BUT5,     // BACK
    BUT_PINS
};

struct UiInstance
{
    int fd;
    char ledstate;
    char warnlight;
};

int ui_init(void);

#endif
