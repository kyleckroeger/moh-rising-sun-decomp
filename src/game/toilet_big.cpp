// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"

extern "C" void *BIG_locateentryz(void *, const char *, int, int *, int *);

extern "C" void *BIG_locate(void *, const char *);
bool TLT_IsFileInRepository(char *);
void TLT_InsertFileInRepository(char *);

void *TLT_LoadFileFromBigFile(char *bigFile, const char *name, int *size) {
    void *file = 0;
    if (!bigFile)
        return 0;
    const char *base;
    int offset;
    if (!name)
        base = 0;
    else {
        const char *last = name - 1;
        for (int i = 0; name[i]; i++) {
            char c = name[i];
            if (c == '\\')
                last = name + i;
            if (c == '/')
                last = name + i;
        }
        base = last + 1;
    }
    if (BIG_locateentryz(bigFile, base, 0, &offset, size))
        file = BIG_locate(bigFile, base);
    return file;
}

void TLT_UnLoadLevelRelocationFile() {
    g_toilet.rtcFile.DeallocateArchiveBuffer();
}

void *TLT_LoadFromRepository(const char *name, int *size, bool *inRepository) {
    char *file = (char *)TLT_LoadFileFromLevelBigFile(name, size);
    *inRepository = false;
    if (file) {
        *inRepository = TLT_IsFileInRepository(file);
        if (!*inRepository)
            TLT_InsertFileInRepository(file);
    }
    return file;
}
