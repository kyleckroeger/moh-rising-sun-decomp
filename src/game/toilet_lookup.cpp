// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"
#include <string.h>

void DebugMsg(const char *, ...);

struct SystemFileEntry {
    const char *name;
    void *data;
    int size;
};
extern SystemFileEntry g_systemFileData[];

void *TLT_GetPauseBigFile() {
    return g_toilet.pauseBigFile;
}

void *TLT_LoadFileFromSystemBigFile(const char *name, int *size) {
    int foundSize = 0;
    void *found = 0;
    for (int i = 0; i <= 5; i++) {
        if (strcmp(name, g_systemFileData[i].name) == 0) {
            found = g_systemFileData[i].data;
            foundSize = g_systemFileData[i].size;
            break;
        }
    }
    if (size)
        *size = foundSize;
    return found;
}

void *TLT_LoadFileFromLevelBigFile(const char *name, int *size) {
    void *file = TLT_LoadFileFromBigFile(g_toilet.levelBigFile, name, size);
    if (!file)
        DebugMsg("!!!! File %s not found in level bigfile\n", name);
    return file;
}

LevelFileContentsStruct_ *TLT_FindNextResourceByType(TLTResourceID type, LevelFileContentsStruct_ *previous) {
    if (previous >= g_toilet.contentsEnd)
        return 0;
    if (!previous)
        previous = g_toilet.contents;
    else
        previous++;
    for (LevelFileContentsStruct_ *entry = previous; entry < g_toilet.contentsEnd; entry++) {
        if (entry->type == type)
            return entry;
    }
    return 0;
}

