// REAL abort messages: format the message, then hand it to the abort hook
// (REAL_abortmessage) or print it with the recorded file and line and exit
// (SYSTEM_abortmessage).
#include <stdarg.h>

/* inferred: va_start written as a copy of the record __builtin_saveregs builds,
   so the copy is inline; the header's __builtin_memcpy form calls memcpy here. */
#define VA_START(ap) (*(ap) = *(__va_list_tag*)__builtin_saveregs())

extern "C" {
int vsprintf(char*, const char*, va_list);
enum PRINTCHANNEL {};
void PRINT_string(PRINTCHANNEL, const char*, ...);
void REAL_exit(void);
extern void (*REALabortmessage)(const char*, ...);
extern char* REALabortfilename;
extern int REALabortlinenum;

void REAL_abortmessage(const char* format, ...) {
    va_list args;
    char buffer[512];

    if (format) {
        VA_START(args);
        vsprintf(buffer, format, args);
        va_end(args);
    } else {
        buffer[0] = 0;
    }
    REALabortmessage("%s", buffer);
}

void SYSTEM_abortmessage(const char* format, ...) {
    va_list args;
    char buffer[512];

    if (format) {
        VA_START(args);
        vsprintf(buffer, format, args);
        va_end(args);
    } else {
        buffer[0] = 0;
    }
    PRINT_string((PRINTCHANNEL)2, "ERROR: %s", buffer);
    if (REALabortfilename)
        PRINT_string((PRINTCHANNEL)2, "FILE %s LINE %d\n", REALabortfilename, REALabortlinenum);
    REAL_exit();
}
}
