#ifndef ABSTRACT_DISPLAY_H
#define ABSTRACT_DISPLAY_H

#include "device.h"

struct Point {
    int x;
    int y;
};

class AbstractDisplay /*: public Device */{
    public:
    virtual ~AbstractDisplay() = default;

    virtual void update_screen() = 0;
//    virtual void set_background_color(int color) = 0;
//    virtual void draw_pixel(Point point, int color) = 0;
};

#endif