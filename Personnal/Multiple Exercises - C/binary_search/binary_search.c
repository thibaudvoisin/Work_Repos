#include <stddef.h>
#include <stdio.h>

int binary_search(const int vec[], size_t size, int elt)
{
    if (!size)
        return -1;

    unsigned int front = 0;
    unsigned int back = size - 1;
    unsigned int front_bef = -1;
    unsigned int back_bef = -1;

    if ((elt < vec[front]) || (elt > vec[back]))
        return -1;

    while (back > front)
    {
        if ((front_bef == front) && (back_bef == back))
            return -1;

        front_bef = front;
        back_bef = back;

        if (vec[front] == elt)
            return front;

        if (vec[back] == elt)
            return back;

        if (elt > vec[front + (back - front) / 2])
        {
            front += ((back - front)) / 2;
        }
        else if (elt < vec[front + (back - front) / 2])
        {
            back = front + (back - front) / 2;
        }
        else
        {
            return (front + ((back - front) / 2));
        }
    }

    if (back == front)
        return back;
    return -1;
}
