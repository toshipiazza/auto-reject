#include "test.h"

int foo(void)
{
    int x = 0;
    int i[] = { 1, 2, 3, 4, 5 };
    for (int j : i)
        x += j;
    return x;
}
