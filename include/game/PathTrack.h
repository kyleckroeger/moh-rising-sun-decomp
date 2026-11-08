// The music path player's project states and tracks. pfstates, pfstate, inited,
// paused, volscale, pfactive, songprogress, bankservice and milliseconds come from
// symbols; 4 states of 540 bytes from pfstates' size. The view types, their member
// names and the parameter meanings are inferred from their uses.
#ifndef PATHTRACK_H
#define PATHTRACK_H

#include <stdio.h>
#include <string.h>
struct PATHNODEVIEW;
struct PATHQUEUEVIEW {
    int node;
    int length;
    int beat;
    int request;
};

struct PATHTRACKVIEW {
    unsigned char index;
    signed char control;
    unsigned char paused;
    signed char fadeVolume;
    signed char volume;
    signed char loopcount;
    signed char beat;
    unsigned char unknown07;
    unsigned char queued;
    unsigned char queueindex;
    unsigned short latency;
    short loopnode;
    short event;
    PATHNODEVIEW* current;
    signed char baseVolume;
    unsigned char savedMode;
    short node;
    unsigned char savedVolume;
    unsigned char mode;
    short restoreNode;
    unsigned char unknown1c[4];
    int beatlength;
    int length;
    int start;
    int time;
    int xtrack;
    int request;
    char name[160];
    PATHQUEUEVIEW* queue;
};

struct PATHBRANCHVIEW {
    signed char low;
    signed char high;
    short node;
};

struct PATHNODEVIEW {
    short sample;
    unsigned char track;
    unsigned char beat;
    unsigned char beatnum;
    unsigned char beatlen;
    signed char sync;
    signed char step;
    signed char first;
    signed char loops;
    unsigned char route;
    unsigned char branches;
    PATHBRANCHVIEW branch[1];
};

struct PATHMAPVIEW {
    unsigned int magic;
    unsigned int version;
    unsigned char unknown08[4];
    unsigned char voice;
    unsigned char columns;
    unsigned char beats;
    unsigned char events;
    unsigned char cellCount;
    unsigned char routeCount;
    unsigned short nodes;
    unsigned char unknown14[16];
    short nodemapStart[1];
};

struct PATHEVENTVIEW {
    short node;
    unsigned char active;
    signed char type;
    unsigned char started;
    int bit;
    int value0c;
    int value10;
    int pending;
};

struct PATHCELLVIEW {
    unsigned char flags;
    unsigned char kind;
    short node;
};

struct PATHSAMPLEVIEW {
    int offset;
    int length;
};

struct PATHSTATEVIEW {
    int leader;
    int latency;
    int window;
    int timerWindow;
    int fadeTime;
    unsigned char unknown14;
    unsigned char unknown15[3];
    unsigned int id;
    PATHMAPVIEW* map;
    short* nodemap;
    short* nodemapEnd;
    unsigned char* table28;
    PATHCELLVIEW* cells;
    int* routes;
    int* columnsData;
    PATHSAMPLEVIEW* samples;
    PATHEVENTVIEW events[16];
    PATHTRACKVIEW* tracks[24];
};

extern PATHSTATEVIEW* pfstate;
extern void (*songprogress)(unsigned int, int);
extern signed char inited;
extern unsigned char paused;
extern signed char volscale;
extern unsigned int pfactive;
extern PATHSTATEVIEW pfstates[4];

extern unsigned char bankservice;
extern int milliseconds;

extern "C" {
int PATHX_milliseconds(void);
void PATHI_servicetask(void);
void PATHI_servicetimer(void);
int PATHI_destroyall(void);
int PATHI_init(void);
unsigned int PATHI_switchvoice(unsigned int voice);
void PATHX_initservice(void (*service)(), void (*timer)(), void (*cleanup)());
struct SNDPLAYOPTS;
int PATHX_createtrack(char* filename, char* name, SNDPLAYOPTS* opts, int count, int streamSize, void* memory, int memorySize, char* bankData);
int PATHX_readyfornewrequest(int track);
void PATHX_modifyhold(int track, int hold);
void PATHI_setvolumes(int mask);
void PATHI_seeknextnode(int index);
void PATHI_eventdeadline(unsigned int handle, int node, int value);
int PATHI_eventstart(int index);
int PATHI_eventnode(int beat, unsigned char* index, PATHCELLVIEW* out);
int PATHI_choosebeat(PATHNODEVIEW* node, PATHNODEVIEW* sync, int* start, int* end);
void PATH_fade(unsigned int handle, int volume, int time);
void PATH_volume(unsigned int handle, int volume);
int PATHX_queuefile(int track, int user, char* name, int size, int volume, int start, int purge);
int PATHI_sampleoffset(int node);
int PATHI_timetonextstreambeat(PATHTRACKVIEW* track, int* length);
int PATHI_timeremaining(PATHTRACKVIEW* track);
void PATHI_eventrestore(int event);
int PATHX_status(int track, int* position, int* remaining);
int PATHX_timeremaining(int track, int req);
int PATHI_queuenode(PATHTRACKVIEW* track, int flag);
short PATHI_enternode(int from, int node, int value);
void PATHI_getmastertrack(void);
void PATHI_service(int index, int master);
int PATHI_eventstate(int beat, int index, PATHCELLVIEW* cell);
int PATHX_stop(int track);
int PATHX_pause(int track, int pause);
int PATHX_volume(int track, int volume);
int PATHX_fade(int track, int volume, int time);
void PATHX_destroytrack(int track);
void PATHX_endservice(void);
unsigned int PATHI_switchproject(unsigned int index);
PATHTRACKVIEW* PATHI_gettrackptr(unsigned int handle);
int PATHI_numtracks(unsigned int handle);
void PATHI_sortprojects(void);
PATHTRACKVIEW* PATH_destroy(unsigned int handle);
int PATHX_getvolume(int track);
int PATHI_routenode(int from, int node);
int PATHI_nextnode(int node, int value);
}

#endif
