// REAL timer set-up: a periodic OS alarm posts a message to the timer thread,
// which runs the registered timer handlers.
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

void AlarmHandler(OSAlarm*, OSContext*);

void InitAlarm() {
    if (!bIsAlarmInited) {
        OSInitAlarm();
        bIsAlarmInited++;
    }
}

extern "C" int TIMER_init(int hz);

extern "C" void TIMER_restore(void);

void ttDoTimerMsg();

void AlarmHandler(OSAlarm*, OSContext*);
