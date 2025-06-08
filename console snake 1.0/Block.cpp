#include "Block.h"
#include "Globals.h"

void block::set(int lr, int du)
{
    lastx = x;
    lasty = y;
    x = lr;
    y = du;
}
void block::move(int lr, int du)
{
    lastx = x;
    lasty = y;
    if (x == getFieldSizeLink() - 1 && lr == 1)
    {
        x = 0;
    }
    else
    {
        if (x == 0 && lr == -1)
        {
            x = getFieldSizeLink() - 1;
        }
        else
        {
            x = x + lr;
        }
    }
    if (y == getFieldSizeLink() - 1 && du == 1)
    {
        y = 0;
    }
    else
    {
        if (y == 0 && du == -1)
        {
            y = getFieldSizeLink() - 1;
        }
        else
        {
            y = y + du;
        }
    }
}
block::block(int a, int b)
{
    active = 0;
    x = a;
    y = b;
    lastx = x;
    lasty = y;
}
void block::spawn(int a, int b)
{
    active = 1;
    x = a;
    y = b;
}