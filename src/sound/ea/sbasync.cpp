// Asynchronous sound-bank loading (sbasync.c). SNDBANKI_asynccompletereads and
// SNDBANKI_asyncissuereads run a ring of four file reads,
// SNDBANKI_asyncxferhdr copies completed reads into the bank header, and
// SNDBANKI_asynccompletedownload and SNDBANKI_asyncissuedownloads move the rest
// to sound memory. SNDBANKI_asyncservice waits for the file to open and the
// first read, allocates the header and data and hands over to
// SNDBANKI_asyncserver, which closes the file when everything is down and
// resolves the bank. SNDBANK_asyncloadi lays a load out in the caller's memory
// and claims a bank slot. The function names and sndbas come from the symbols;
// the load, read, slot and sndgs views are inferred from offsets. The loops are
// written with an explicit guard and a bottom test, which is the shape in the
// original instructions.
extern "C" {
void SNDI_memcpy(void*, const void*, int);
int FILESYS_completeop(int);
int FILESYS_opstatus(int);
int FILESYS_read(int, int, void*, int, int, void*);
}

struct SndBankReadView {
    int op;
    int download;
    char* buffer;
    signed char state;
    unsigned char unknown0d[3];
};

struct SndBankHeaderView {
    unsigned char unknown00[8];
    int headersize;
    int datasize;
};

struct SndBankLoadView {
    int state;
    int aborted;
    int opened;
    int background;
    int openOp;
    int closeOp;
    SndBankHeaderView* header;
    void* (*alloc)(int);
    int fileOffset;
    int file;
    int partial;
    int size;
    int headerRemaining;
    char* dest;
    int destOffset;
    int remaining;
    unsigned short chunkSize;
    unsigned short bank;
    unsigned short issued;
    unsigned short completed;
    unsigned short downloaded;
    unsigned short downloadBase;
    SndBankReadView reads[4];
};

struct SndBankAsyncView {
    SndBankLoadView* load;
    int priority;
    int error;
};

extern SndBankAsyncView sndbas;

struct HANDLE_def;

/* inferred: the sound globals' bank count and slots */
struct SndBankSlotView {
    int header;
    char* dest;
    unsigned char loading;
    unsigned char unknown09[3];
};

struct SndGlobalsView {
    unsigned char unknown000[40];
    unsigned short bankcount;
    unsigned char unknown02a[458];
    SndBankSlotView* banks;
};

extern SndGlobalsView sndgs;

void SNDBANKI_abortload();
void SNDBANKI_asyncresolve();
void SNDBANKI_asyncservice();
void SNDBANKI_asyncserver();
int SNDPLATFORM_downloadcomplete(int);
int SNDPLATFORM_download(int, void*, void*, int);
void* SNDPLATFORM_memalloc(int, int);
void iSNDserverremoveclient(void (*)());
extern "C" {
int FILESYS_close(HANDLE_def*, int, void*);
void SNDSYS_remove100hzclient(void (*)());
void SNDSYS_add100hzclient(void (*)());
}

void SNDBANKI_asynccompletereads() {
    SndBankReadView* read;
    int i = sndbas.load->completed % 4;

    if (sndbas.load->issued > sndbas.load->completed) {
        do {
            read = &sndbas.load->reads[i];
            if (read->state != 1)
                break;
            if (FILESYS_opstatus(read->op) != 1)
                break;
            if (FILESYS_completeop(read->op) > 0)
                read->state = 2;
            else
                read->state = 0;
            i++;
            sndbas.load->completed++;
            if (i >= 4)
                i = 0;
        } while (sndbas.load->issued > sndbas.load->completed);
    }
}

void SNDBANKI_asyncissuereads(void) {
    SndBankLoadView* load;
    SndBankReadView* read;
    int i;
    int size = 0xFFFFFF;
    int offset;

    i = sndbas.load->issued % 4;
    if (sndbas.load->completed > sndbas.load->issued - 4) {
        do {
            load = sndbas.load;
            offset = load->issued * load->chunkSize;
            if (load->header) {
                size = load->size - offset;
                if (size <= 0)
                    break;
            }
            if (size > load->chunkSize)
                size = load->chunkSize;
            read = &load->reads[i];
            if (read->state)
                break;
            read->op = FILESYS_read(load->file, load->fileOffset + offset, read->buffer, size, sndbas.priority, 0);
            read->state = 1;
            i++;
            sndbas.load->issued++;
            if (i >= 4)
                i = 0;
        } while (sndbas.load->completed > sndbas.load->issued - 4);
    }
}

void SNDBANKI_asyncxferhdr(void) {
    SndBankLoadView* load;
    int size;
    SndBankReadView* read;
    int i;

    if (sndbas.load->headerRemaining <= 0)
        return;
    i = sndbas.load->downloaded % 4;
    if (sndbas.load->downloadBase < sndbas.load->completed) {
        do {
            load = sndbas.load;
            read = &load->reads[i];
            if (load->headerRemaining < load->chunkSize)
                size = load->headerRemaining;
            else
                size = load->chunkSize;
            SNDI_memcpy((char*)sndbas.load->header + sndbas.load->chunkSize * sndbas.load->downloadBase, read->buffer, size);
            sndbas.load->headerRemaining -= size;
            if (sndbas.load->headerRemaining <= 0 && size < sndbas.load->chunkSize) {
                sndbas.load->partial = size;
                return;
            }
            read->state = 0;
            i++;
            sndbas.load->downloaded++;
            sndbas.load->downloadBase++;
            if (i >= 4)
                i = 0;
        } while (sndbas.load->downloadBase < sndbas.load->completed);
    }
}

void SNDBANKI_asynccompletedownload(void) {
    SndBankReadView* read;
    int i = sndbas.load->downloadBase % 4;

    if (sndbas.load->downloaded > sndbas.load->downloadBase) {
        do {
            read = &sndbas.load->reads[i];
            if (!SNDPLATFORM_downloadcomplete(read->download))
                break;
            if (read->download <= 0)
                break;
            read->state = 0;
            i++;
            sndbas.load->downloadBase++;
            if (i >= 4)
                i = 0;
        } while (sndbas.load->downloaded > sndbas.load->downloadBase);
    }
}

void SNDBANKI_asyncissuedownloads(void) {
    SndBankReadView* read;
    int i;
    int size;

    if (sndbas.load->remaining <= 0)
        return;
    i = sndbas.load->downloadBase % 4;
    if (sndbas.load->downloaded < sndbas.load->downloadBase + 4) {
        do {
            int offset = i * sizeof(SndBankReadView);
            SndBankLoadView* load = sndbas.load;

            read = (SndBankReadView*)((char*)load->reads + offset);
            if (read->state != 2)
                return;
            if (load->remaining <= 0) {
                read->state = 0;
                return;
            }
            if (load->partial < load->chunkSize) {
                size = load->chunkSize - load->partial;
                if (size > load->remaining)
                    size = load->remaining;
                read->download = SNDPLATFORM_download(0x200, read->buffer + load->partial, load->dest + load->destOffset, size);
                read->state = 3;
                sndbas.load->destOffset += size;
                sndbas.load->remaining -= size;
            }
            if (read->state == 3) {
                sndbas.load->partial = 0;
                sndbas.load->downloaded++;
            }
            i++;
            if (i >= 4)
                i = 0;
        } while (sndbas.load->downloaded < sndbas.load->downloadBase + 4);
    }
}

void SNDBANKI_asyncprocess(void) {
    SNDBANKI_asynccompletereads();
    SNDBANKI_asyncxferhdr();
    SNDBANKI_asynccompletedownload();
    SNDBANKI_asyncissuedownloads();
    SNDBANKI_asyncissuereads();
}

void SNDBANKI_asyncserver(void) {
    SNDBANKI_asyncprocess();
    if (sndbas.load->headerRemaining <= 0 && sndbas.load->remaining <= 0) {
        if (!sndbas.load->closeOp)
            sndbas.load->closeOp = FILESYS_close((HANDLE_def*)sndbas.load->file, sndbas.priority, 0);
        if (sndbas.load->downloaded == sndbas.load->downloadBase && FILESYS_opstatus(sndbas.load->closeOp) == 1) {
            FILESYS_completeop(sndbas.load->closeOp);
            SNDSYS_remove100hzclient(SNDBANKI_asyncserver);
            SNDBANKI_asyncresolve();
            sndbas.load = 0;
        }
    }
}

void SNDBANKI_asyncservice(void) {
    int aborted = sndbas.load->aborted;

    if (aborted) {
        SNDBANKI_abortload();
        return;
    }
    if (sndbas.load->openOp) {
        int status = FILESYS_opstatus(sndbas.load->openOp);

        if (status != 1) {
            if (status != 0) {
                FILESYS_completeop(sndbas.load->openOp);
                sndbas.error = -19;
                sndbas.load->aborted = 1;
                SNDBANKI_abortload();
            }
            return;
        }
        sndbas.load->file = FILESYS_completeop(sndbas.load->openOp);
        sndbas.load->openOp = aborted;
        sndbas.load->opened = status;
        SNDBANKI_asyncissuereads();
    }
    {
        SndBankLoadView* load = sndbas.load;
        SndBankReadView* read = &load->reads[0];
        int status = FILESYS_opstatus(load->reads[0].op);

        if (status != 1)
            return;
        FILESYS_completeop(load->reads[0].op);
        read->state = 2;
        sndbas.load->completed++;
        sndbas.load->header = (SndBankHeaderView*)read->buffer;
        sndbas.load->headerRemaining = sndbas.load->header->headersize;
        sndbas.load->size = sndbas.load->header->headersize;
        sndbas.load->remaining = sndbas.load->header->datasize;
        sndbas.load->size += sndbas.load->header->datasize;
        sndbas.load->header = (SndBankHeaderView*)sndbas.load->alloc(sndbas.load->headerRemaining);
        {
            SndBankLoadView* current = sndbas.load;
            int remaining;

            if (!current->header)
                goto failed;
            remaining = current->remaining;
            if (!remaining)
                goto nodata;
            sndbas.load->dest = (char*)SNDPLATFORM_memalloc(512, remaining + 64);
            current = sndbas.load;
            if (current->dest)
                goto loaded;
failed:
            current->aborted = status;
            sndbas.error = -6;
            return;
nodata:
            current->dest = (char*)remaining;
        }
loaded:
        iSNDserverremoveclient(SNDBANKI_asyncservice);
        sndbas.load->state = 0;
        SNDBANKI_asyncprocess();
        SNDSYS_add100hzclient(SNDBANKI_asyncserver);
    }
}

void iSNDserveraddclient(void (*)());
extern "C" int FILESYS_open(const char* name, int mode, int priority, void* callbackData);

int SNDBANK_asyncloadi(char* name, int offset, void* memory, int size, void* (*alloc)(int), bool background) {
    int i;

    sndbas.load = (SndBankLoadView*)memory;
    sndbas.error = 0;
    ((SndBankLoadView*)memory)->aborted = 0;
    sndbas.load->opened = 0;
    sndbas.load->background = background;
    sndbas.load->alloc = alloc;
    sndbas.load->fileOffset = offset;
    sndbas.load->partial = 0;
    sndbas.load->size = 0;
    sndbas.load->headerRemaining = 0;
    sndbas.load->destOffset = 0;
    sndbas.load->header = 0;
    sndbas.load->issued = 0;
    sndbas.load->completed = 0;
    sndbas.load->downloaded = 0;
    sndbas.load->downloadBase = 0;
    sndbas.load->closeOp = 0;
    sndbas.load->chunkSize = (unsigned int)(size - 460) >> 2;
    if (sndbas.load->chunkSize > 0x6000)
        sndbas.load->chunkSize = 0x6000;
    sndbas.load->chunkSize &= ~0x3f;
    sndbas.load->openOp = FILESYS_open(name, 1, sndbas.priority, 0);
    for (i = 0; i < 4; i++) {
        SndBankReadView* read = &sndbas.load->reads[i];

        read->download = -1;
        read->state = 0;
        read->buffer = (char*)(((sndbas.load->chunkSize + 64) * i + (unsigned int)memory + 268) & ~63);
    }
    for (i = 0; i < sndgs.bankcount; i++) {
        SndBankSlotView* slot = &sndgs.banks[i];

        if (slot->header)
            continue;
        slot->loading = 1;
        slot->header = 1;
        sndbas.load->bank = i;
        break;
    }
    iSNDserveraddclient(SNDBANKI_asyncservice);
    sndbas.load->state = 1;
    SNDBANKI_asyncservice();
    if (!sndbas.load)
        return sndbas.error;
    return sndbas.load->bank;
}

extern "C" int SNDBANK_asyncload(char* name, int offset, void* memory, int size, void* (*alloc)(int)) {
    return SNDBANK_asyncloadi(name, offset, memory, size, alloc, false);
}

extern "C" int SNDBANK_asyncdone(void) {
    if (!sndbas.load) {
        if (sndbas.error)
            return sndbas.error;
        return 1;
    }
    return 0;
}
