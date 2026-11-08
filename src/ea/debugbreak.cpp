// DEBUG_break (the REAL runtime's debug layer): panics when a debug exit has been requested. The function and debugexit
// are named by their symbols; the meaning of debugexit is inferred.
extern "C" void OSPanic(const char*, int, const char*, ...);

extern int debugexit;

extern "C" void DEBUG_break(void) {
    if (debugexit)
        OSPanic("", 0, "");
}
