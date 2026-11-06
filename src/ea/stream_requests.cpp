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

static int validatehandle(int handle, STREAMHEADERtag** header, TAPSTRUCTtag** tap) {
    STREAMHEADERtag* owner;

    if (!handle)
        return 1;
    owner = ((TAPSTRUCTtag*)handle)->header;
    if (owner->magic != 0x4D525453)
        return 1;
    *tap = (TAPSTRUCTtag*)handle;
    *header = owner;
    return 0;
}

static int inbetween(char* start, char* end, char* address) {
    if (start <= end) {
        if (address < start)
            return 0;
        return address < end;
    }
    {
        int result = 0;

        if (address >= start || address < end)
            result = 1;
        return result;
    }
}

static void decbufferusage(STREAMHEADERtag* header, int amount) {
    int before;
    int after;

    MUTEX_lock(header->mutex);
    before = header->buffered;
    after = before - amount;
    header->buffered = after;
    MUTEX_unlock(header->mutex);
    if (before >= header->threshold && after < header->threshold) {
        header->greedy = 1;
        if (header->state == 1)
            FILESYS_priorityop(header->fileOp, header->priority);
    }
}

extern int requestidcounter;

static REQUESTSTRUCTtag* getfreerequest(STREAMHEADERtag* header) {
    REQUESTSTRUCTtag* request = 0;

    MUTEX_lock(header->mutex);
    if (header->free) {
        request = header->free;
        header->free = request->next;
        requestidcounter += 256;
        if (!requestidcounter)
            requestidcounter = 256;
        request->index = (request->index & 0xff) | requestidcounter;
    }
    MUTEX_unlock(header->mutex);
    return request;
}

static void queuerequest(STREAMHEADERtag* header, REQUESTSTRUCTtag* request) {
    request->state = 1;
    request->next = 0;
    MUTEX_lock(header->mutex);
    if (!header->last) {
        request->prev = header->last;
        header->first = request;
        header->current = request;
        header->last = request;
    } else {
        request->prev = header->last;
        header->last->next = request;
        header->last = request;
    }
    MUTEX_unlock(header->mutex);
}

static REQUESTSTRUCTtag* locaterequest(STREAMHEADERtag* header, int id) {
    int index = id & 0xff;
    REQUESTSTRUCTtag* request;

    if (index >= header->requestCount)
        return 0;
    request = &header->requests[index];
    if (id != request->index)
        return 0;
    if (!request->state)
        return 0;
    return request;
}

static void freerequest(STREAMHEADERtag* header, REQUESTSTRUCTtag* request) {
    if (request == header->first)
        header->first = request->next;
    else
        request->prev->next = request->next;
    if (request == header->last)
        header->last = request->prev;
    else
        request->next->prev = request->prev;
    if (request == header->current) {
        if (!request->next)
            header->current = request->prev;
        else
            header->current = request->next;
    }
    request->state = 0;
    request->next = header->free;
    header->free = request;
}

static int filterchunk(STREAMHEADERtag* header, STREAMCHUNKHDR* chunk) {
    int id = STREAM_GET32(chunk->id);
    int i;

    for (i = 0; i < header->typeCount; i++) {
        CHUNKTYPEVIEW* entry = &header->types[i];

        if ((id & entry->mask) == entry->value)
            return entry->type;
    }
    return -2;
}

int parsechunks(STREAMHEADERtag* header);

void opencallback(int, int, void* data);

void closecallback(int, int, void* data);

void readcallback(int, int, void* data);

void startnextrequest(STREAMHEADERtag* header, int priority);

void restartstream(STREAMHEADERtag* header, int priority);

/* inferred: the bytes a stream needs besides its buffer (header, request,
   chunk type and tap tables, and 32 for aligning the buffer) */

extern "C" int STREAM_overhead(int requests, int types, int taps);

extern "C" int STREAM_create(int requests, int types, int taps, void* memory, int size);

/* inferred: a valid handle is a non-null tap whose header carries the
   stream magic */

extern "C" void STREAM_setfilter(int handle, int type, int mask, int value, int tap);

extern "C" void STREAM_destroy(int handle);
