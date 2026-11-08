// AI-assisted reconstruction from GR8E69. Part of the original Moh3.cpp. A loaded module image
// holds its entry point at +0x18, a section table offset at +0x1c and a section count at +0x2c.
// Section record members are inferred from use.
#include "MohLoader.h"
#include <string.h>

extern "C" {
void VISetBlack(int black);
void VIFlush();
void VIWaitForRetrace();
void GXAbortFrame();
void DCFlushRangeNoSync(void *start, unsigned int length);
void ICInvalidateRange(void *start, unsigned int length);
void PPCSync();
void OSResetSystem(int reset, unsigned int resetCode, int forceMenu);
void CPU_detect();
int TIMER_init(int rate);
void THREAD_init();
void SYNCTASK_init();
void SYNCTASK_add(void (*task)(int, int), int, int);
void SYNCTASK_del(void (*task)());
void PAD_init();
void PAD_update();
void *DWI_alloc(const char *, int, int);
int DWI_free(void *);
void FILESYS_setmemcallbacks(void *(*alloc)(const char *, int, int), int (*release)(void *));
int FILESYS_init(int handles, int bufferSize, int operations);
void ASYNCFILE_init(int count, int flags);
void ASYNCFILE_restore();
void FILESYS_restore();
void THREAD_restore();
void TIMER_restore();
void PAD_restore();
void FONT_restore();
void REAL_restore();
}
void CheckDiscDriveSyncTask(int, int);

struct ModuleSection {
    unsigned char unknown00[4];
    int fileOffset;     // +0x04
    char *address;      // +0x08
    unsigned char unknown0c[4];
    unsigned int fileSize; // +0x10
    int memorySize;     // +0x14
    unsigned char unknown18[8];
};
struct ModuleImage {
    unsigned char unknown00[0x18];
    void (*entry)();    // +0x18
    int sectionTable;   // +0x1c
    unsigned char unknown20[0x2c - 0x20];
    short sectionCount; // +0x2c
};

class CSprite {
public:
    enum ESpriteLoadType { SpriteLoadUnknown0 = 0, SpriteLoadUnknown1 = 1, SpriteLoadUnknown2 = 2 };
    CSprite(const char *name, ESpriteLoadType type);
    ~CSprite();
    unsigned char unknown[36];
};
class CStaticScreen {
public:
    void DrawLoadingScreen(CSprite &sprite);
};
extern CStaticScreen g_StaticScreen;
class CMoviePlayer {
public:
    static void InitClass();
};

void MohLoaderGc::LoadModule(const void *image, unsigned long size) {
    const ModuleImage *module = (const ModuleImage *)image;
    if (module && size) {
        VISetBlack(1);
        VIFlush();
        VIWaitForRetrace();
        GXAbortFrame();
        VIWaitForRetrace();
        const ModuleSection *section = (const ModuleSection *)((const char *)module + module->sectionTable);
        int count = module->sectionCount;
        for (int i = 0; i < count; i++) {
            void *address = section->address;
            int memorySize = section->memorySize;
            int fileSize = (section->fileSize + 31) & ~31;
            memset(address, 0, memorySize);
            if (fileSize > 0)
                memcpy(address, (const char *)module + section->fileOffset, fileSize);
            memorySize = (memorySize + 31) & ~31;
            section++;
            DCFlushRangeNoSync(address, memorySize);
            ICInvalidateRange(address, memorySize);
        }
        PPCSync();
        OSResetSystem(2, 0x80000000, 0);
        SetReturningFlag();
        module->entry();
    }
}

void GcDisplayLoadingScreen(const char *name) {
    CSprite sprite(name, CSprite::SpriteLoadUnknown2);
    g_StaticScreen.DrawLoadingScreen(sprite);
}

void InitREAL() {
    CPU_detect();
    TIMER_init(100);
    THREAD_init();
    SYNCTASK_init();
    SYNCTASK_add(CheckDiscDriveSyncTask, -1, 0);
    PAD_init();
    SYNCTASK_del(PAD_update);
    FILESYS_setmemcallbacks(DWI_alloc, DWI_free);
    FILESYS_init(0, 1200, 0);
    ASYNCFILE_init(6, 0);
    CMoviePlayer::InitClass();
}

void ShutdownREAL() {
    ASYNCFILE_restore();
    FILESYS_restore();
    THREAD_restore();
    TIMER_restore();
    PAD_restore();
    FONT_restore();
    REAL_restore();
}
