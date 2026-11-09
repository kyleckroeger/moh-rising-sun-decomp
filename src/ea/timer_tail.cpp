// REAL timer: the tick queries. ticks, tickval and TIMERhz are small statics of
// REAL, declared extern as original context.
extern "C" {
extern int ticks;
extern int TIMERhz;
}
extern int tickval;

extern "C" int TIMER_gettick(void) {
    return ticks;
}

extern "C" int TIMER_elapsedticks(void) {
    int last = tickval;
    int now = TIMER_gettick();
    tickval = now;
    return now - last;
}

extern "C" int TIMER_getfrequency(void) {
    return TIMERhz;
}
