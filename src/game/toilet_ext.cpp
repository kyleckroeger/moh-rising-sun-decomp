// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"
#include <stdio.h>
#include <string.h>

extern "C" int strcasecmp(const char *, const char *);
extern "C" int BIG_count(void *);
extern "C" void *BIG_locateentry(void *, const char *, int, int *, int *);

class TLT_FindExtData {
public:
    void Reset() {
        name[0] = 0;
        extension[0] = 0;
        index = 0;
        extensionLength = 0;
    }
    void SetExt(char *ext);
    bool MatchExtension(const char *entry);

    char name[256];      // +0x000
    char extension[256]; // +0x100
    int extensionLength; // +0x200
    int index;           // +0x204
};
bool TLT_FindNextExtension(TLT_FindExtData &data);

void TLT_FindExtData::SetExt(char *ext) {
    if (ext && ext[0]) {
        if (ext[0] == '.')
            strcpy(extension, ext);
        else
            sprintf(extension, ".%s", ext);
    } else {
        extension[0] = 0;
    }
    extensionLength = strlen(extension);
}

bool TLT_FindExtData::MatchExtension(const char *entry) {
    int length = strlen(entry);
    if (entry && length > extensionLength && strcasecmp(entry + (length - extensionLength), extension) == 0)
        return true;
    return false;
}

bool TLT_FindFirstExtension(TLT_FindExtData &data, char *ext) {
    data.Reset();
    if (ext && ext[0]) {
        data.SetExt(ext);
        return TLT_FindNextExtension(data);
    }
    return false;
}

bool TLT_FindNextExtension(TLT_FindExtData &data) {
    int count = BIG_count(g_toilet.levelBigFile);
    while (data.index < count) {
        const char *entry = (const char *)BIG_locateentry(g_toilet.levelBigFile, 0, data.index, 0, 0);
        data.index++;
        if (data.MatchExtension(entry)) {
            strcpy(data.name, entry);
            return true;
        }
    }
    return false;
}
