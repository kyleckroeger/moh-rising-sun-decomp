// The pathx layer's track table and the sound calls it uses. pathxinfo, the
// PATHX_* names and SNDPLAYOPTS come from symbols; 24 slots of 64 bytes come from
// pathxinfo's symbol size. PathxTrackView's members, other than their offsets, and
// the parameter meanings are inferred from their uses.
#ifndef PATHXTRACK_H
#define PATHXTRACK_H

#include <stdio.h>

struct SNDPLAYOPTS {
    unsigned char volume;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
    unsigned char byte5;
    unsigned short short6;
    unsigned short short8;
    short shortA;
    unsigned short pitch;
    unsigned short shortE;
    unsigned short short10;
    unsigned short short12;
    unsigned short short14;
    unsigned short short16;
};

struct PathxTrackView {
    int stream;
    int handle;
    int request;
    int patch;
    int count;
    int requestedVolume;
    unsigned short pitch;
    unsigned char isSound;
    char name[9];
    char* bankData;
    SNDPLAYOPTS opts;
};

struct SNDSAMPLEFORMAT {
    unsigned short rate;
    unsigned char channels;
    unsigned char type;
};
struct SNDSAMPLEATTR {
    int unknown[26];
};
struct SNDSAMPLEDESC {
    unsigned int samples;
    int unknown[7];
};

extern PathxTrackView pathxinfo[24];
extern int (*pathabortmsg)(const char*, ...);
extern int (*pathprintf)(const char*, ...);
extern void (*pathService)();
extern void (*pathTimer)();
extern void (*pathCleanup)();

extern "C" {
int FILE_size(const char* name);
void* MEM_allocz(const char* name, int size, int flags);
int MEM_free(void*);
bool FILESYS_opensync(const char*, int, int, int*);
int FILESYS_readsync(int, int, void*, int, int);
bool FILESYS_closesync(int, int);
int SNDbankadd(int* bank, void* data);
int SNDbankheadersize(int);
int SNDbankheadercopy(void* buffer, int bank);
int SNDbankremove(int);
int SNDplaysetdef(SNDPLAYOPTS* opts);
int SNDSTRM_create(SNDPLAYOPTS* opts, int requests, int players, void* memory, int size);
int SNDSTRM_destroy(int);
int SNDSTRM_overhead(int channels, int buffers);
int SNDSTRM_status(int stream, int* status);
int SNDSTRM_requeststatus(int request, unsigned int* status);
int SNDSTRM_getprogvol(int stream);
int SNDSTRM_purge(int index);
int SNDSTRM_queuefile(int index, int user, char* name, int size);
int SNDSTRM_modifyhold(int request, int hold);
int SNDSTRM_autovol(int stream, int time, int volume);
int SNDSTRM_vol(int stream, int volume);
int SNDSTRM_pitchmult(int stream, int multiplier);
int SNDCTRL_getprogvol(int handle);
int SNDtimeremaining(int handle);
int SNDBANK_patchinfo(int bank, int patch, SNDSAMPLEFORMAT* format, SNDSAMPLEATTR* attr, SNDSAMPLEDESC* desc);
int SNDBANK_play(int bank, int patch, SNDPLAYOPTS* opts);
int SNDautovol(int handle, int time, int volume);
int SNDvol(int handle, int volume);
int SNDpitchmult(int handle, int multiplier);
int SNDstop(int handle);
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
void SNDSYS_add100hzclient(void* client);
void SNDSYS_remove100hzclient(void* client);
void SYNCTASK_add(int (*)(int, int), int, int);
void SYNCTASK_del(int (*)(int, int));
void REAL_abortmessage(const char*, ...);
int TIMER_gettick(void);
extern int TIMERhz;
int PATHX_volume(int track, int volume);
}

int PATHX_igetslot();
int PATHX_iloadbank(int track, char* filename, char* data);
char* PATHX_iloadfile(char* filename);
int PATHX_isynctask(int, int);

#endif
