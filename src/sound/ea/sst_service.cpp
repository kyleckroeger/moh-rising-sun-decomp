// Sound streams: stream lookup and teardown, the chunk release callback, the
// frames callback that advances the current request (finishing it, or after
// 200 passes, onto the stream's finished list) and carries surplus frames to
// the next one, and the header parser, which moves to the next request,
// converts the chunk's patch into the pending sample format and attributes,
// reports the attributes' events to the registered callbacks, releases the
// chunk, and then adopts the new format (stalling, state 2, when a playing
// stream's format changes) and starts the packet player; SNDSTRMI_parsedata
// builds a packet from a data chunk (channel pointers from the chunk's
// offsets, the chunk stashed before the first channel's data for the release
// callback) and submits it, and SNDSTRMI_isheld reports whether the current
// request is still held for prebuffering; SNDSTRMI_service, for each stream
// with queued requests, restarts a stalled player once it has drained,
// and unless the stream is held reads up to the player's free space (or 10)
// chunks, parsing data and header chunks. SNDSTREAMCHANNEL, SNDSAMPLEFORMAT,
// SNDSAMPLEATTR, SNDSAMPLEDESC, SNDLINKNODE, SNDLINKLIST, STREAMCHUNKHDR and
// TAGGEDPATCH are named by the mangled symbols; their members, the stream
// records and sndgs/sndss are inferred views. SNDSTRMI_create takes a free
// stream slot, lays the stream record, its request nodes, the packet
// player's memory and the stream buffer out in the caller's memory, creates
// the packet player and (unless an external stream handle is given) the
// stream, and registers the service client with the first stream; the
// packet pointer passing through the 'created' local is an inferred form
// that reproduces the original's register use.
#include "SoundGlobals.h"
extern "C" void* sndss[];

struct TAGGEDPATCH;

/* inferred members */
struct SNDPLAYOPTS {
    unsigned char unknown00;
    unsigned char unknown01;
    unsigned char unknown02;
    unsigned char unknown03;
    unsigned char unknown04;
    unsigned char unknown05;
    unsigned short unknown06;
    unsigned short unknown08;
    short unknown0a;
    unsigned short unknown0c;
    unsigned short unknown0e;
    unsigned short unknown10;
    unsigned short unknown12;
    unsigned short unknown14;
    unsigned short unknown16;
};
struct STREAMCHUNKHDR;

/* inferred members */
struct SNDSAMPLEFORMAT {
    unsigned short rate;
    unsigned char channels;
    unsigned char type;
};

/* inferred members */
struct SNDSAMPLEATTR {
    short unknown00;
    unsigned char unknown02;
    unsigned char unknown03;
    unsigned char unknown04;
    unsigned char unknown05;
    unsigned char unknown06;
    unsigned char unknown07;
    unsigned short unknown08;
    unsigned short unknown0a;
    int unknown0c[3];
    void* buffer[6];
    int size[6];
    unsigned int event[4];
    int eventData[4];
};

/* inferred: filled in by the patch conversion, starting with the sample's
   frame count */
struct SNDSAMPLEDESC {
    unsigned int frames;
    unsigned char unknown04[24];
};

void SNDI_patchtohdr(void*, TAGGEDPATCH*, SNDSAMPLEFORMAT*, SNDSAMPLEATTR*, SNDSAMPLEDESC*, unsigned char*);

extern "C" {
int SNDSTRM_destroy(int);
void STREAM_release(int, unsigned char*);
int memcmp(const void*, const void*, unsigned long);
int SNDPKTPLAY_start(int, SNDSAMPLEFORMAT*, SNDSAMPLEATTR*, SNDPLAYOPTS*);
void SNDCTRL_filteradd(int, void*);
void* STREAM_gettable(int);
int STREAM_state(int);
void SNDPKTPLAY_submit(int, void*);
int SNDPKTPLAY_framesoutstanding(int);
int SNDPKTPLAY_submitspace(int);
void SNDPKTPLAY_stop(int);
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
unsigned char* STREAM_get(int);
void* memset(void*, int, unsigned long);
int SNDPKTPLAY_overhead(int);
int SNDPKTPLAY_create(void (*)(void*, void*), void (*)(int, int, void*), void*, void*, int);
void SNDPKTPLAY_destroy(int);
void* STREAM_create(int, int, int, void*, int);
int STREAM_buffersize(int);
void SNDSTRM_setgreedylevel(int, int);
int SNDSTRM_purge(int);
}

// SNDSTRMI_calcdatarate: a format's data rate in bytes per second, the
// sample rate times the channels times the codec's bits per 256 samples
// (16-bit PCM, type 16, is simply 8 bytes per channel). The locals are
// declared in the order channels, rate, bits but assigned rate, bits,
// channels (register order).
int SNDSTRMI_calcdatarate(SNDSAMPLEFORMAT* format);

int SNDSTRMI_destroyall(void);

void* SNDSTRMI_getstreamptr(int handle);

void SNDSTRMI_releasecallback(void* data, void*);

/* inferred: a queued stream request, linked by its first fields (the frame
   count left and the hold time are volatile: the original rereads them
   after comparing them) */
struct SNDLINKNODE {
    SNDLINKNODE* next;
    unsigned char unknown04[8];
    int handle;
    unsigned int rate;
    volatile unsigned int done;
    volatile unsigned int total;
    volatile unsigned int left;
    volatile int hold;
    unsigned char started;
    unsigned char patchdata[1];
};

/* inferred: a list of request nodes */
struct SNDLINKLIST {
    SNDLINKNODE* head;
    SNDLINKNODE* tail;
    int count;
};

/* the stream record: named by the mangled symbols, members inferred */
struct SNDSTREAMCHANNEL {
    int handle;
    int voice;
    volatile int player;
    int unknown0c;
    signed char state;
    unsigned char external;
    unsigned char unknown012[2];
    SNDSAMPLEFORMAT format;
    SNDSAMPLEFORMAT newFormat;
    SNDSAMPLEATTR attr;
    SNDSAMPLEATTR newAttr;
    SNDPLAYOPTS opts;
    unsigned char filter[16];
    void* filterSet;
    unsigned char unknown0f0[4];
    SNDLINKLIST active;
    SNDLINKLIST finished;
    SNDLINKNODE* current;
};

SNDLINKNODE* SNDSTRMI_getrequestptr(int);
void SNDLINKI_remove(SNDLINKLIST*, SNDLINKNODE*);
void SNDLINKI_push(SNDLINKLIST*, SNDLINKNODE*);
void SNDLINKI_init(SNDLINKLIST*);
void iSNDserveraddclient(void (*)(void));

/* inferred: moves a request to the stream's finished list */
int SNDSTRMI_removerequest(int handle);

void SNDSTRMI_framescallback(int, int frames, void* data);

/* inferred: the notification passed to the registered callbacks */
struct SNDSTREAMEVENT {
    int type;
    int event;
    int data;
    int unknown0c;
    int handle;
};


void SNDSTRMI_startstream(SNDSTREAMCHANNEL*);
int SNDSTRMI_numcreated(void);

int SNDSTRMI_parseheader(int index, STREAMCHUNKHDR* chunk);

/* inferred: a packet for the packet player: a frame count, a flag marking a
   request's first packet, and the channels' sample data */
struct SNDSTREAMPACKET {
    int unknown00;
    unsigned int frames : 31;
    unsigned int first : 1;
    int unknown08;
    unsigned char* data[6];
};

void SNDSTRMI_parsedata(SNDSTREAMCHANNEL* stream, STREAMCHUNKHDR* chunk);

int SNDSTRMI_parsechunk(int index, STREAMCHUNKHDR* chunk) {
    SNDSTREAMCHANNEL* stream = (SNDSTREAMCHANNEL*)sndss[index];

    if (*(int*)chunk == 0x5343446C) {
        SNDSTRMI_parsedata(stream, chunk);
    } else if (*(int*)chunk == 0x5343486C) {
        SNDSTRMI_parseheader(index, chunk);
        return 0;
    } else {
        STREAM_release(stream->handle, (unsigned char*)chunk);
    }
    return 1;
}

int SNDSTRMI_isheld(SNDSTREAMCHANNEL* stream) {
    SNDLINKNODE* request = stream->current;
    unsigned int buffered;

    if (!request)
        return 0;
    if (!request->rate)
        return 0;
    if (request->hold < 0)
        return 1;
    if (request->hold) {
        buffered = (unsigned int)STREAM_gettable(stream->handle);
        if (buffered > 4000000)
            buffered = 4000000;
        if (buffered * 1000 / request->rate >= request->hold) {
            request->hold = 0;
            return 0;
        }
        if (STREAM_state(stream->handle) == 2) {
            request->hold = 0;
            return 0;
        }
        if (stream->finished.count <= 0 && STREAM_state(stream->handle) == 0) {
            request->hold = 0;
            return 0;
        }
        return 1;
    }
    return 0;
}

/* inferred: hands a chunk to the data or header parser (or releases an
   unknown one); false when a header was parsed */

void SNDSTRMI_service(void) {
    SNDSTREAMCHANNEL* stream;
    int i;
    int more;
    int count;
    STREAMCHUNKHDR* chunk;

    SNDSYS_entercritical();
    for (i = 0; i < sndgs.packetplayercount; i++) {
        stream = (SNDSTREAMCHANNEL*)sndss[i];
        if (!stream || !stream->active.count)
            continue;
        if (stream->state == 2) {
            if (SNDPKTPLAY_framesoutstanding(stream->player) > 0)
                continue;
            stream->format = stream->newFormat;
            stream->attr = stream->newAttr;
            stream->newAttr.buffer[0] = 0;
            SNDPKTPLAY_stop(stream->player);
            SNDSTRMI_startstream(stream);
        }
        if (SNDSTRMI_isheld(stream))
            continue;
        if (stream->state == 1) {
            count = SNDPKTPLAY_submitspace(stream->player);
            if (count <= 0)
                continue;
        } else {
            count = 8;
        }
        more = 0;
        do {
            count--;
            chunk = (STREAMCHUNKHDR*)STREAM_get(stream->handle);
            if (chunk)
                more = SNDSTRMI_parsechunk(i, chunk);
        } while (more && count > 0);
    }
    SNDSYS_leavecritical();
}

int SNDSTRMI_numcreated(void) {
    int i;
    int count = 0;

    for (i = 0; i < sndgs.packetplayercount; i++) {
        if (sndss[i])
            count++;
    }
    return count;
}

int SNDSTRMI_create(SNDPLAYOPTS* opts, int requests, int players, void* memory, int size, int handle,
                    int external);

// SNDSTRMI_queue and the stream functions after it are not reconstructed.
