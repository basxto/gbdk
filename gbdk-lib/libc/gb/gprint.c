#include <gb/drawing.h>

#pragma bank=BASE

void gprint(char *str)
{
    while(*str)
	wrtchr(*str++);
}
