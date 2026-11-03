// REAL timer: the alarm handler that posts a message to the timer thread, and
// the tick queries that end the file. ticks, tickval and TIMERhz are small
// statics of REAL, declared extern as original context; the message's enum is
// named by the mangled ttMsg symbol, and its value 0 is the only one seen here.
struct OSAlarm;
struct OSContext;
enum TIMERMSG { TIMERMSG_TICK };

void ttMsg(TIMERMSG);

extern "C" {
extern int ticks;
extern int TIMERhz;
}
extern int tickval;

static void AlarmHandler(OSAlarm*, OSContext*) {
    ttMsg(TIMERMSG_TICK);
}

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
