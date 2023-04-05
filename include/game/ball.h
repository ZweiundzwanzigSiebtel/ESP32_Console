#ifndef BALL_H
#define BALL_H

class Ball {
    public:
    Ball();
    void move();
    void draw();
    void reset();
    void set_speed(int speed);
    int get_speed();
    int get_position();
    void set_directionX(int directionX);
    void set_directionY(int directionY);
    int get_directionX();
    int get_directionY();

    private:
    int x;
    int y;
    int speed;
    int directionX;
    int directionY;
};

#endif