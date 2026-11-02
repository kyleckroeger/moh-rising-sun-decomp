// A fragment of the packet player (0x8015ab48): submitting a packet (copied
// into the next free slot with a sequence number, its channels' data counted
// as queued), the free packet slots and outstanding frames, stopping a player
// (stopping its voice, flushing the deferred callbacks and freeing the
// channel buffers) and destroying it, fetching a channel's next packet (frame
// count and first flag out, the primary channel also releasing the oldest
// delivered packet and moving its frames from queued to pending), returning
// played frames, and running the deferred frames-done and release callbacks.
// SNDPKTPLAY_start before it is not reconstructed. Player records and sndpps
// are inferred views; the sequence number, queued counts, packet count and
// released-packet index are volatile (the original rereads them, or loads
// them in source order).
/* inferred: a packet as queued in a player */
struct SNDPKTPLAYPACKET {
    int sequence;
    int frames : 31;
    int first : 1;
    void* data[6];
};

/* inferred: a packet as submitted (the stream layer's packet) */
struct SNDPKTSUBMIT {
    int unknown00;
    unsigned int frames : 31;
    unsigned int first : 1;
    int unknown08;
    void* data[6];
};

struct SNDPKTPLAYER {
    int handle;
    volatile int sequence;
    volatile unsigned int releasedcount;
    volatile unsigned int consumedcount;
    volatile unsigned int readcount[6];
    volatile short read[6];
    volatile short queued[6];
    volatile short maxpackets;
    signed char primary;
    signed char active;
    volatile short released;
    volatile short write;
    volatile unsigned int frames;
    volatile unsigned int pending;
    void* memory;
    void (*release)(void*, void*);
    void (*framesdone)(int, int, void*);
    void* data;
    char field38[2];
    unsigned char channels;
    char field3B;
    void* buffers[6];
    SNDPKTPLAYPACKET packets[1];
};

/* inferred: a deferred callback (type 0: frames done, 1: packet released) */
struct SNDPKTCALLBACK {
    unsigned short type;
    unsigned short player;
    int data;
};

struct SNDPPSVIEW {
    int count;
    SNDPKTCALLBACK callbacks[96];
    SNDPKTPLAYER* players[1];
};

extern "C" {
extern SNDPPSVIEW sndpps;
void SNDstop(int);
}
void SNDPKTPLAYI_flushcallbackdata(void);
void SNDMEMI_free(void*);
void SNDPLATFORM_packetplaydestroy(int);

extern "C" int SNDPKTPLAY_submit(int index, SNDPKTSUBMIT* submit) {
    SNDPKTPLAYER* player = sndpps.players[index];
    SNDPKTPLAYPACKET* packet;
    int i;
    int sequence;

    for (i = 0; i < player->channels; i++) {
        if (player->queued[i] >= player->maxpackets - 1)
            return -13;
    }
    packet = &player->packets[player->write];
    packet->frames = submit->frames;
    packet->first = submit->first;
    packet->sequence = player->sequence;
    for (i = 0; i < player->channels; i++) {
        packet->data[i] = submit->data[i];
        player->queued[i]++;
    }
    player->frames += submit->frames;
    sequence = player->sequence;
    player->sequence++;
    player->write++;
    if (player->write >= player->maxpackets)
        player->write = 0;
    return sequence;
}

extern "C" int SNDPKTPLAY_submitspace(int index) {
    SNDPKTPLAYER* player = sndpps.players[index];
    int most = 0;
    int i;

    for (i = 0; i < player->channels; i++) {
        if (player->queued[i] >= most)
            most = player->queued[i];
    }
    return player->maxpackets - most - 1;
}

extern "C" int SNDPKTPLAY_framesoutstanding(int index) {
    SNDPKTPLAYER* player = sndpps.players[index];

    return player->frames + player->pending;
}

extern "C" int SNDPKTPLAY_purge(int, int, int) {
    return -15;
}

extern "C" int SNDPKTPLAY_stop(int index) {
    SNDPKTPLAYER* player = sndpps.players[index];
    int i;

    SNDstop(player->handle);
    SNDPKTPLAYI_flushcallbackdata();
    SNDPKTPLAY_purge(index, 0, 0x7fffffff);
    player->handle = -1;
    for (i = 0; i < player->channels; i++) {
        if (player->buffers[i])
            SNDMEMI_free(player->buffers[i]);
    }
    return 0;
}

extern "C" int SNDPKTPLAY_destroy(int index) {
    SNDPLATFORM_packetplaydestroy(index);
    sndpps.players[index] = 0;
    return 0;
}

/* inferred: defers a callback until the next flush */
static inline void addcallback(int type, int index, int data) {
    SNDPKTCALLBACK* callback = &sndpps.callbacks[sndpps.count];

    callback->type = type;
    callback->player = index;
    callback->data = data;
    sndpps.count++;
}

/* inferred: releases the oldest packet not yet released */
static inline void releasepacket(SNDPKTPLAYER* player, int index) {
    SNDPKTPLAYPACKET* packet = &player->packets[player->released];

    if (player->release) {
        SNDPKTCALLBACK* callback = &sndpps.callbacks[sndpps.count];

        callback->type = 1;
        callback->player = index;
        callback->data = (int)packet->data[0];
        sndpps.count++;
    }
    player->released++;
    if (player->released >= player->maxpackets)
        player->released = 0;
}

void* SNDPKTPLAYI_get(int index, int channel, int* frames, int* first) {
    SNDPKTPLAYER* player = sndpps.players[index];
    SNDPKTPLAYPACKET* packet;
    int i = 0;
    int ready = 1;
    int consumed = 1;

    for (; i < player->channels; i++) {
        if (player->readcount[i] <= player->releasedcount)
            ready = 0;
    }
    if (ready) {
        SNDPKTPLAYPACKET* oldest = &player->packets[player->released];

        if (player->release) {
            SNDPKTCALLBACK* entry = &sndpps.callbacks[sndpps.count];

            entry->type = 1;
            entry->player = index;
            entry->data = (int)oldest->data[0];
            sndpps.count++;
        }
        player->released++;
        player->releasedcount++;
        if (player->released >= player->maxpackets)
            player->released = 0;
    }
    if (player->queued[channel] == 0)
        return 0;
    packet = &player->packets[player->read[channel]];
    i = 0;
    *frames = packet->frames;
    *first = packet->first;
    player->read[channel]++;
    player->readcount[channel]++;
    do {
        if (player->readcount[i] <= player->consumedcount)
            consumed = 0;
        i++;
    } while (consumed == 1 && i < player->channels);
    if (consumed) {
        player->consumedcount++;
        player->frames -= packet->frames;
        player->pending += packet->frames;
    }
    if (player->read[channel] >= player->maxpackets)
        player->read[channel] = 0;
    player->queued[channel]--;
    {
        void* result = (void*)-1;
        void* data = packet->data[channel];

        if (data)
            result = data;
        return result;
    }
}

void SNDPKTPLAYI_freeframes(int index, int channel, int frames) {
    SNDPKTPLAYER* player = sndpps.players[index];

    if (channel != player->primary)
        return;
    player->pending -= frames;
    if (player->framesdone)
        addcallback(0, index, frames);
}

void SNDPKTPLAYI_flushcallbackdata(void) {
    int i;

    for (i = 0; i < sndpps.count; i++) {
        SNDPKTPLAYER* player;
        SNDPKTCALLBACK* callback = &sndpps.callbacks[i];

        player = sndpps.players[callback->player];

        if (callback->type == 0)
            player->framesdone(callback->player, callback->data, player->data);
        else
            player->release((void*)callback->data, player->data);
    }
    sndpps.count = 0;
}
