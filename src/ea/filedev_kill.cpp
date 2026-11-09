// A fragment of EA's file device (filedev.cpp): killfiledev cancels the read
// thread. ReadFileThread is named by its symbol; the thread's type comes from
// the Dolphin SDK.
struct OSThread;

extern "C" void OSCancelThread(OSThread* thread);

extern OSThread ReadFileThread;

void killfiledev() {
    OSCancelThread(&ReadFileThread);
}
