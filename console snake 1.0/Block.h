#pragma once

class block
{
public:
    block(int a = 0, int b = 0);
    bool active;
    int x;
    int y;
    int lastx;
    int lasty;
    void set(int, int);
    void move(int, int);
    void spawn(int, int);
};