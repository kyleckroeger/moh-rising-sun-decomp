// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"
#include <string.h>

extern "C" int ASYNCFILE_load(const char *name, int size);
extern "C" int ASYNCFILE_getstatus(int id);
extern "C" int ASYNCFILE_release(int id, char **buffer, int *done);
extern "C" bool FILE_exists(const char *name);

float TLT_Async_PercentComplete() {
    return g_toilet.asyncPercent;
}

const char *TLT_Async_CurrentFilename() {
    return g_toilet.asyncFilename;
}

void TLT_LoadFile_Async_Start(const char *name, int flags) {
    strcpy(g_toilet.asyncFilename, name);
    g_toilet.asyncRequest = ASYNCFILE_load(name, flags | 0x400);
}

bool TLT_LoadFile_Async_Load(char **buffer, int *size) {
    Toilet *toilet = &g_toilet;
    int status = ASYNCFILE_getstatus(toilet->asyncRequest);
    if (status == 1) {
        char *loaded;
        int loadedSize;
        status = ASYNCFILE_release(toilet->asyncRequest, &loaded, &loadedSize);
        toilet->asyncRequest = 0;
        if (buffer)
            *buffer = loaded;
        if (size)
            *size = loadedSize;
    } else {
        if (buffer)
            *buffer = 0;
        if (size)
            *size = 0;
    }
    return status != 1;
}

bool TLT_FileExists(const char *name) {
    if (*name)
        return FILE_exists(name);
    return false;
}
