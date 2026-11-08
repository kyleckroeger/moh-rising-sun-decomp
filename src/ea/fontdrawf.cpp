// FONT_drawtextfa: formatted text, drawn through FONT_drawtexta.
#include <stdarg.h>

/* inferred: va_start written as a copy of the record __builtin_saveregs builds,
   so the copy is inline; the header's __builtin_memcpy form calls memcpy here. */
#define VA_START(ap) (*(ap) = *(__va_list_tag*)__builtin_saveregs())

#include "Font.h"
extern "C" {
int vsprintf(char*, const char*, va_list);

void FONT_drawtextfa(FONT* font, float x, float y, const unsigned char* format, ...) {
    va_list args;
    char buffer[8192];

    VA_START(args);
    vsprintf(buffer, (const char*)format, args);
    va_end(args);
    FONT_drawtexta(font, x, y, (const unsigned char*)buffer);
}
}
