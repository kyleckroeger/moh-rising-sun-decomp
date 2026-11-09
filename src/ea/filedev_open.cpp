// The file device's open path. openfile opens a host file through the PC
// functions (handle type 3 in closefile's terms) when the mode's bit 0 is clear
// or the name starts with "hd:\\", "hd:/" or "hd:" (compared case-insensitively),
// converting backslashes; otherwise it opens the disc file by path (type 2).
// HANDLE_def and the helpers are named by the mangled symbols; the handle's
// members are inferred views.
#include <ctype.h>

struct DVDFileInfo {
    unsigned char unknown00[60];
};

struct HANDLE_def {
    int type;
    char name[260];
    union {
        int file;
        DVDFileInfo dvd;
    };
};

extern "C" {
int PCopen(const char*, int, int);
int PCcreat(const char*, int);
int PCclose(int);
void REAL_abortmessage(const char*, ...);
long DVDConvertPathToEntrynum(const char*);
int DVDFastOpen(long, DVDFileInfo*);
int DVDClose(DVDFileInfo*);
}

static int openfile_plat(const char* name, unsigned int mode, int* file) {
    switch (mode & 7) {
    case 0:
        *file = PCopen(name, 2, 0);
        break;
    case 1:
        *file = PCopen(name, 0, 0);
        break;
    case 2:
        *file = PCopen(name, 2, 0);
        if (*file == -1)
            *file = PCcreat(name, 0);
        break;
    case 6:
        *file = PCcreat(name, 0);
        break;
    default:
        REAL_abortmessage("openfile - ILLEGAL OPEN MODE (%d).\n", mode);
        *file = -1;
        return 0;
    }
    return *file != -1;
}

static void FixSlash(char* dst, const char* src) {
    int i;

    for (i = 0; src[i]; i++) {
        if (src[i] == '\\')
            dst[i] = '/';
        else
            dst[i] = src[i];
    }
    dst[i] = src[i];
}

/* returns the prefix's length when name starts with it, ignoring case, else 0 */
static int MyStrCmpi(const char* name, const char* prefix) {
    int i;
    char c;

    for (i = 0;; i++) {
        c = prefix[i];
        if (c == 0)
            return i;
        if (tolower(name[i]) != c)
            break;
    }
    return 0;
}

int openfile(const char* name, unsigned int mode, HANDLE_def* handle) {
    char path[256];
    const char* prefixes[3] = {"hd:\\", "hd:/", "hd:"};
    int host = mode ^ 1;
    DVDFileInfo* info = &handle->dvd;
    int result;
    int length = 0;

    host &= 1;
    if (!host) {
        unsigned int i;
        for (i = 0; i <= 2; i++) {
            length = MyStrCmpi(name, prefixes[i]);
            if (length) {
                host = 1;
                break;
            }
        }
    }
    if (host) {
        FixSlash(path, name + length);
        result = openfile_plat(path, mode, &handle->file);
        if (!result)
            PCclose(handle->file);
        else
            handle->type = 3;
    } else {
        long entry;
        int k;
        char* p = path;

        for (k = 0; name[k]; k++) {
            if (name[k] == '\\')
                path[k] = '/';
            else
                path[k] = name[k];
        }
        p[k] = name[k];
        entry = DVDConvertPathToEntrynum(p);
        if (entry == -1) {
            result = 0;
        } else {
            result = DVDFastOpen(entry, info);
            if (!result)
                DVDClose(&handle->dvd);
        }
        if (result)
            handle->type = 2;
    }
    return result;
}
