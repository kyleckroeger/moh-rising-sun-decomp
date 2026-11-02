// This file group has no name in the original symbols; the file name is Frontline's.
// REAL exit handlers: up to 64 functions, run in reverse order of slot on
// restore or exit.
extern "C" {
void exit(int);

static void (*exitfunctions[64])(void) = { 0 };

void REAL_restore(void) {
    for (int i = 63; i >= 0; i--) {
        if (exitfunctions[i])
            exitfunctions[i]();
        exitfunctions[i] = 0;
    }
}

void REAL_exit(void) {
    REAL_restore();
    exit(0);
}

void REAL_addexit(void (*function)(void));

void REAL_removeexit(void (*function)(void));
}
