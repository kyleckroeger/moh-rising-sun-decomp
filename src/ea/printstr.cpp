// PRINT_string: format through PRINT_vstring. A fragment of the print library.
#include <stdarg.h>

/* inferred: va_start written as a copy of the record __builtin_saveregs builds,
   so the copy is inline; the header's __builtin_memcpy form calls memcpy here. */
#define VA_START(ap) (*(ap) = *(__va_list_tag*)__builtin_saveregs())

extern "C" {
enum PRINTCHANNEL {};
void PRINT_vstring(PRINTCHANNEL channel, const char* format, va_list args);

void PRINT_string(PRINTCHANNEL channel, const char* format, ...) {
    va_list args;

    VA_START(args);
    PRINT_vstring(channel, format, args);
    va_end(args);
}
}
