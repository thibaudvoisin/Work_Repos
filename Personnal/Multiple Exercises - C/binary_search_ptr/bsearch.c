#include <stddef.h>

int *binary_search(int *begin, int *end, int elt)
{
    size_t size = end - begin;
    unsigned int front = 0;
    unsigned int back = size - 1;
    unsigned int front_bef = -1;
    unsigned int back_bef = -1;

    if (!size)
        return begin;
    if (elt < *(begin + front))
        return begin;
    if (elt > *(begin + back))
        return end;

    while (back > front)
    {
        if ((front_bef == front) && (back_bef == back))
            return begin + front + 1;

        front_bef = front;
        back_bef = back;

        if (*(begin + front) == elt)
            return begin + front;

        if (*(begin + back) == elt)
            return begin + back;

        if (elt > *(begin + (front + (back - front) / 2)))
        {
            front += ((back - front)) / 2;
        }
        else if (elt < *(begin + front + ((back - front) / 2)))
        {
            back = front + (back - front) / 2;
        }
        else
        {
            return begin + (front + ((back - front) / 2));
        }
    }
    if (back == front)
        return begin + back;
    return begin + front;
}
