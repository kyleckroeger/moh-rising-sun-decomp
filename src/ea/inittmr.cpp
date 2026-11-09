// REAL timer set-up: a periodic OS alarm posts a message to the timer thread,
// which runs the registered timer handlers.
/* inferred: low memory as far as used here */
struct OSLOWMEM {
    unsigned char unknown00[248];
    unsigned int busclock;
};

struct OSAlarm {
    unsigned char unknown00[40];
};
struct OSContext;
typedef long long OSTime;
enum TIMERMSG {};

extern "C" {
void OSInitAlarm(void);
OSTime OSGetTime(void);
void OSSetPeriodicAlarm(OSAlarm*, OSTime, OSTime, void (*)(OSAlarm*, OSContext*));
void OSCancelAlarm(OSAlarm*);
void REAL_addexit(void (*)(void));
extern int TIMERhz;
extern volatile int ticks;
extern volatile int libticks;
extern void (*tmrsub[8])(void);
void TIMER_restore(void);
}

void ttInit();
void ttKill();
void ttMsg(TIMERMSG);

extern OSAlarm Alarm;
extern char bIsAlarmInited;
extern char bIsTimerInited;

static void AlarmHandler(OSAlarm*, OSContext*);

void InitAlarm() {
    if (!bIsAlarmInited) {
        OSInitAlarm();
        bIsAlarmInited++;
    }
}

extern "C" int TIMER_init(int hz) {
    if (bIsTimerInited)
        return TIMERhz;

    if (hz == 0)
        hz = 100;
    TIMERhz = hz;

    OSTime now = OSGetTime();
    OSTime period = (int)(((OSLOWMEM*)0x80000000)->busclock / 4 * (1.0f / hz));
    ttInit();
    OSSetPeriodicAlarm(&Alarm, now, period, AlarmHandler);
    bIsTimerInited = 1;
    REAL_addexit(TIMER_restore);
    return TIMERhz;
}

extern "C" void TIMER_restore(void) {
    if (bIsTimerInited) {
        bIsTimerInited = 0;
        OSCancelAlarm(&Alarm);
        ttKill();
    }
}

void ttDoTimerMsg() {
    ticks++;
    libticks++;
    for (int i = 0; i < 8; i++) {
        if (tmrsub[i])
            tmrsub[i]();
    }
}

static void AlarmHandler(OSAlarm*, OSContext*) {
    ttMsg((TIMERMSG)0);
}
