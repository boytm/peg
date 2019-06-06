#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State
{
    ANY,
    CR,
};

enum State count_state = ANY;
int pos = 0;
int *lines = NULL;
int linepos = 0;
int linelen = 0;

void pushLine(int pos)
{
    // printf("add line, pos %d\n", pos);
    while (linelen <= linepos)
    {
        linelen = linelen ? linelen * 2 : 1;
        lines = (int *)realloc(lines, sizeof(int) * linelen);
    }
    lines[linepos++] = pos;
}

int getLine(int pos, int *column)
{
    int retval;
    int from, to, mid;
    for (from = 0, to = linepos; from + 1 < to;)
    {
        mid = (from + to) / 2;
        if (pos < lines[mid])
        {
            to = mid;
        }
        else if (pos == lines[mid])
        {
            break;
        }
        else /* pos > lines[mid] */
        {
            from = mid;
        }
    }
    // printf("pos %d, from line %d pos %d, mid line %d pos %d, to line %d pos
    // %d\n", pos, from, lines[from], mid, lines[mid], to, lines[to]);

    if (pos <= lines[from])
        retval = from - 1;
    else if (pos <= lines[mid])
        retval = mid - 1;
    else if (pos <= lines[to])
        retval = to - 1;
    else
        retval = to;

    if (column)
    {
        if (retval == -1)
        {
            *column = pos - (-1);
        }
        else
        {
            *column = pos - lines[retval];
        }
    }

    return retval + 2;
}

int countChar(const char c)
{
    switch (count_state)
    {
    case ANY:
        if (c == '\r')
        {
            count_state = CR;
        }
        else if (c == '\n')
        {
            pushLine(pos);
        }
        break;
    case CR:
        if (c == '\r')
        {
            pushLine(pos - 1);
            count_state = CR;
        }
        else if (c == '\n')
        {
            pushLine(pos);
            count_state = ANY;
        }
        else
        {
            pushLine(pos - 1);
            count_state = ANY;
        }
        break;
    default:
        printf("error state %d", count_state);
        return -1;
    }
    ++pos;
    return 0;
}

int countChars(const char *buf, size_t len)
{
    for (int i = 0; i < len; ++i)
    {
        if (countChar(buf[i]) < 0)
        {
            return -1;
        }
    }
    return 0;
}

#if 0
int main(int argc, char **argv)
{
    int i, ret;
    char buf[4096];
    FILE *f = fopen(argv[1], "rb");
    do {
        ret = fread(buf, 1, sizeof(buf), f);
        if (ret < 0)
            printf("read error");
        countChars(buf, ret);
    } while(ret > 0);

    for (i=0; i < 100; ++i)
    {
        int column = 0;
        int line = getLine(i, &column);
        printf("pos %d, line %d:%d\n", i, line, column);
    }
    return 0;
}
#endif
