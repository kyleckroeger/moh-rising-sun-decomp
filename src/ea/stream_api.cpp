// This file group has no name in the original symbols; the file name is Frontline's.
/* A fragment of EA's streaming layer (stream.cpp): the request
   list, the chunk parser, the file operation callbacks, the buffer refill
   and STREAM_overhead. freerequest unlinks a request from the queue (moving
   the current request on, or back when it was the last) and puts it on the
   free list. parsechunks walks the chunks written for the current request:
   a chunk whose size word has a high byte is a raw read and gets a header
   naming the request; the request's own chunks are padded to 32 bytes; the
   id is matched against the chunk type table (id & mask == value), unknown
   types are marked free (-2) and known ones get their type in the size
   word's high byte and are added to their tap's list and the buffered
   count, leaving greedy mode when the count crosses the threshold. It
   returns 1 when it reaches the request's end chunk, and stops (leaving the
   chunk unclaimed) once the request is cancelled (state 4). opencallback
   records the opened handle and restarts the stream; closecallback reopens
   the stream's file and installs opencallback; readcallback adds the bytes
   read to the file position and the write pointer, parses the new chunks,
   and then starts the next request when this one is finished (its whole
   file read, or a short read, or chunks parsed), marking it done, or
   restarts the stream; startnextrequest moves to the next queued request
   (or marks the stream idle), resets the read position, and switches files
   (closing the open one, or opening the new one) when the request names
   another file, else restarts the stream. restartstream skips consumed
   chunks (an id of -1 wraps to the chunk start, -2 skips its length), frees
   finished requests whose data left the unread part of the ring, finds the
   free space (moving the partial request to the buffer start behind a wrap
   marker when the tail is too short), and then copies the next part of a
   memory request or starts a file read; the stream is stalled (state 2)
   while there is too little room. STREAM_overhead is the memory a stream
   needs besides its buffer; STREAM_create checks the limits (a buffer of at
   least 6 KB, 2..256 requests, 1..16 chunk types, 1..types taps), lays the
   header, request, type and tap tables and the 32-byte aligned buffer out
   in the given memory, picks the minimum read from the buffer size, chains
   the free requests, and returns the first tap as the handle.
   STREAM_setfilter sets a chunk type's match (the last type only matches
   everything) and its tap (or -1/-2) while the stream is idle;
   STREAM_destroy kills the stream, waits (running the sync tasks on the
   owning thread) until it stops reading, clears the magic, and closes the
   file. Handles are checked by the same inline test as in stream.cpp.
   STREAMHEADERtag and REQUESTSTRUCTtag are named by the mangled symbols;
   their members, the other views and the helpers' signatures are
   inferred. */
/* inferred: a stream request (state, type, file size) */
struct REQUESTSTRUCTtag {
    int index;
    int state;
    REQUESTSTRUCTtag* prev;
    REQUESTSTRUCTtag* next;
    int type;
    char filename[256];
    unsigned char* source;
    int size;
    int id;
    unsigned char* start;
};

struct STREAMHEADERtag;

/* inferred: a chunk type match (id & mask == value) */
struct CHUNKTYPEVIEW {
    int mask;
    int value;
    int type;
};

/* inferred: the chunks buffered for one type */
struct TAPSTRUCTtag {
    STREAMHEADERtag* header;
    int number;
    int size;
    unsigned char* first;
};

struct STREAMHEADERtag {
    int magic;
    unsigned char mutex[28];
    REQUESTSTRUCTtag* requests;
    int requestCount;
    CHUNKTYPEVIEW* types;
    int typeCount;
    TAPSTRUCTtag* lists;
    int listCount;
    unsigned char* bufferStart;
    unsigned char* chunkStart;
    unsigned char* bufferEnd;
    int state;
    int unknown48;
    int priority;
    int threshold;
    int greedy;
    int buffered;
    unsigned char* read;
    unsigned char* requestStart;
    unsigned char* write;
    REQUESTSTRUCTtag* first;
    REQUESTSTRUCTtag* current;
    REQUESTSTRUCTtag* last;
    REQUESTSTRUCTtag* free;
    char filename[256];
    int handle;
    int position;
    int fileOp;
    int requested;
    int minimum;
};

extern "C" {
void STREAM_setgreedystate(int, int);
char* strncpy(char*, const char*, unsigned long);
unsigned char* STREAM_get(int);
void STREAM_release(int, unsigned char*);
int FILESYS_completeop(int);
void FILESYS_priorityop(int, int);
int FILESYS_open(const char*, int, int, void*);
void FILESYS_callbackop(int, void (*)(int, int, void*));
int FILESYS_close(int, int, void*);
int FILESYS_read(int, int, void*, int, int, void*);
int strcmp(const char*, const char*);
char* strcpy(char*, const char*);
void MUTEX_lock(void*);
void MUTEX_unlock(void*);
void MEM_copy(void*, const void*, int);
void MEM_clear(void*, int);
void MUTEX_create(void*);
void MUTEX_destroy(void*);
bool THREAD_iscurrent(int);
void SYNCTASK_run(int);
void THREAD_yield(int);
bool FILESYS_closesync(int, int);
void STREAM_kill(int);
}

void restartstream(STREAMHEADERtag*, int);
void startnextrequest(STREAMHEADERtag*, int);

#define STREAM_GET32(p) (((p)[3] << 24) | ((p)[2] << 16) | ((p)[1] << 8) | (p)[0])

/* inferred: stores a little-endian word */
struct STREAMCHUNKHDR {
    unsigned char id[4];
    unsigned char size[4];
};

static inline void put32(unsigned char* p, unsigned int value) {
    p[0] = value;
    p[1] = value >> 8;
    p[2] = value >> 16;
    p[3] = value >> 24;
}

int validatehandle(int handle, STREAMHEADERtag** header, TAPSTRUCTtag** tap);

int inbetween(char* start, char* end, char* address);

void decbufferusage(STREAMHEADERtag* header, int amount);

extern int requestidcounter;

REQUESTSTRUCTtag* getfreerequest(STREAMHEADERtag* header);

void queuerequest(STREAMHEADERtag* header, REQUESTSTRUCTtag* request);

REQUESTSTRUCTtag* locaterequest(STREAMHEADERtag* header, int id);

void freerequest(STREAMHEADERtag* header, REQUESTSTRUCTtag* request);

int filterchunk(STREAMHEADERtag* header, STREAMCHUNKHDR* chunk);

int parsechunks(STREAMHEADERtag* header);

void opencallback(int, int, void* data);

void closecallback(int, int, void* data);

void readcallback(int, int, void* data);

void startnextrequest(STREAMHEADERtag* header, int priority);

void restartstream(STREAMHEADERtag* header, int priority);

/* inferred: the bytes a stream needs besides its buffer (header, request,
   chunk type and tap tables, and 32 for aligning the buffer) */

extern "C" int STREAM_overhead(int requests, int types, int taps) {
    int typesize = types * sizeof(CHUNKTYPEVIEW);
    int tapsize = taps * sizeof(TAPSTRUCTtag);

    int base = typesize + sizeof(STREAMHEADERtag);

    return requests * sizeof(REQUESTSTRUCTtag) + base + tapsize + 32;
}

extern "C" int STREAM_create(int requests, int types, int taps, void* memory, int size) {
    STREAMHEADERtag* header = (STREAMHEADERtag*)memory;
    int buffer = size - STREAM_overhead(requests, types, taps);
    unsigned char* start;
    int i;

    if (buffer < 6144 || requests < 2 || requests > 256 || (unsigned int)(types - 1) > 15 || taps < 1 || taps > types)
        return 0;
    header->magic = 0x4D525453;
    MUTEX_create(header->mutex);
    header->requests = (REQUESTSTRUCTtag*)(header + 1);
    header->requestCount = requests;
    header->types = (CHUNKTYPEVIEW*)(header->requests + requests);
    header->typeCount = types;
    header->lists = (TAPSTRUCTtag*)(header->types + types);
    header->listCount = taps;
    start = (unsigned char*)(((unsigned int)(header->lists + taps) & ~31) + 32);
    header->bufferStart = start;
    header->chunkStart = start;
    header->bufferEnd = (unsigned char*)memory + size;
    header->state = 0;
    header->unknown48 = 150;
    header->priority = 50;
    header->threshold = 0;
    header->greedy = 0;
    header->buffered = 0;
    header->read = start;
    header->requestStart = start;
    header->write = start;
    header->first = 0;
    header->current = 0;
    header->last = 0;
    header->free = header->requests;
    MEM_clear(header->filename, 255);
    header->handle = 0;
    if (buffer < 16384)
        header->minimum = 2048;
    else if (buffer < 32768)
        header->minimum = 4096;
    else if (buffer < 65536)
        header->minimum = 8192;
    else if (buffer < 131072)
        header->minimum = 16384;
    else
        header->minimum = 32768;
    for (i = 0; i < requests; i++) {
        REQUESTSTRUCTtag* request = &header->requests[i];

        request->index = i;
        request->state = 0;
        request->next = &header->requests[i + 1];
    }
    header->requests[requests - 1].next = 0;
    for (i = 0; i < types; i++) {
        CHUNKTYPEVIEW* type = &header->types[i];

        type->mask = 0;
        type->value = 0;
        type->type = 1;
    }
    for (i = 0; i < taps; i++) {
        TAPSTRUCTtag* tap = &header->lists[i];

        tap->header = header;
        tap->number = i + 1;
        tap->size = 0;
    }
    return (int)header->lists;
}

/* inferred: a valid handle is a non-null tap whose header carries the
   stream magic */

extern "C" void STREAM_setfilter(int handle, int type, int mask, int value, int tap) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;
    CHUNKTYPEVIEW* entry;

    if (validatehandle(handle, &header, &stream) != 0)
        return;
    if (type < 1 || type > header->typeCount)
        return;
    if (type == header->typeCount && (mask | value) != 0)
        return;
    if ((tap < 1 && tap != -1 && tap != -2) || tap > header->listCount)
        return;
    if (header->state != 0)
        return;
    entry = &header->types[type - 1];
    entry->mask = mask;
    entry->value = value;
    entry->type = tap;
}

extern "C" void STREAM_destroy(int handle) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;

    if (validatehandle(handle, &header, &stream) != 0)
        return;
    STREAM_kill(handle);
    while (header->state == 1) {
        if (THREAD_iscurrent(0))
            SYNCTASK_run(0);
        THREAD_yield(0);
    }
    header->magic = 0;
    MUTEX_destroy(header->mutex);
    if (header->handle)
        FILESYS_closesync(header->handle, 100);
}

extern "C" void STREAM_setpriority(int handle, int priority, int priorityClass) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;
    if (validatehandle(handle, &header, &stream) == 0) {
        header->unknown48 = priority;
        header->priority = priorityClass;
    }
}

extern "C" void STREAM_setgreedylevel(int handle, int level) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;
    if (validatehandle(handle, &header, &stream) == 0) {
        int old = header->threshold;
        int now;
        int was;
        header->threshold = level;
        now = header->buffered < level;
        was = header->buffered < old;
        if (was != now)
            STREAM_setgreedystate(handle, now);
    }
}

extern "C" void STREAM_setgreedystate(int handle, int state) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;
    if (validatehandle(handle, &header, &stream) == 0) {
        header->greedy = state;
        if (state && header->state == 1)
            FILESYS_priorityop(header->fileOp, header->priority);
    }
}

extern "C" void* STREAM_taphandle(int handle, int tap) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;
    if (validatehandle(handle, &header, &stream) != 0)
        return 0;
    if (tap < 1 || tap > header->listCount)
        return 0;
    return &header->lists[tap - 1];
}

extern "C" int STREAM_queuefile(int handle, const char* name, int size, int id) {
    TAPSTRUCTtag* stream;
    STREAMHEADERtag* header;
    REQUESTSTRUCTtag* request;
    int state;
    int error = validatehandle(handle, &header, &stream);

    if (error)
        return 0;
    request = getfreerequest(header);
    if (request == 0)
        return 0;
    request->type = 0;
    strncpy(request->filename, name, 254);
    request->size = size;
    request->id = id;
    queuerequest(header, request);
    MUTEX_lock(header->mutex);
    state = header->state;
    if (state == 0)
        header->state = 1;
    MUTEX_unlock(header->mutex);
    if (state == 0) {
        if (header->greedy)
            startnextrequest(header, header->priority);
        else
            startnextrequest(header, header->unknown48);
    }
    return request->index;
}

extern "C" int STREAM_queuemem(int handle, unsigned char* data, int size, int id);

extern "C" void STREAM_cancelrequest(int handle, int requestHandle);

extern "C" unsigned char* STREAM_get(int handle);

extern "C" void STREAM_release(int handle, unsigned char* chunk);

extern "C" void* STREAM_gettable(int handle);

extern "C" int STREAM_state(int handle);

extern "C" int STREAM_isendofstream(int handle);

extern "C" int STREAM_buffersize(int handle);
