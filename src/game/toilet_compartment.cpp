// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"
#include <stdio.h>

extern "C" int DWI_free(void *);
extern "C" void *BIG_locateentryz(void *, const char *, int, int *, int *);

void TLT_CloseFile(void *file) {
    if ((char *)file >= g_toilet.levelBigFile && (char *)file < g_toilet.levelBigFile + g_toilet.levelBigFileSize)
        return;
    if ((char *)file >= g_toilet.bigFile120 && (char *)file < g_toilet.bigFile120 + g_toilet.bigFile120Size)
        return;
    if ((char *)file >= g_toilet.pauseBigFile && (char *)file < g_toilet.pauseBigFile + g_toilet.pauseBigFileSize)
        return;
    DWI_free(file);
}

void GetCompartmentNameFromIndex(int index, char *name, int) {
    LevelFileContentsStruct_ *entry = TLT_FindNextResourceByTypeAndVal((TLTResourceID)0, 1, index, 0);
    if (!entry)
        name[0] = 0;
    else
        sprintf(name, "|%s", entry->data[0]->value);
}

void GetCompartmentCDBNameFromIndex(int index, char *name, int size) {
    GetCompartmentNameFromIndex(index * 3, name, size);
}

void GetCompartmentCPTNameFromIndex(int index, char *name, int size) {
    GetCompartmentNameFromIndex(index * 3 + 1, name, size);
}

void GetCompartmentRTCNameFromIndex(int index, char *name, int size) {
    GetCompartmentNameFromIndex(index * 3 + 2, name, size);
}

int GetVisualSizeFromIndex(int index) {
    char name[256];
    int offset, size;
    if (g_toilet.compartmentBigFile) {
        sprintf(name, "%d_%d_Art_c%d.cpt", g_toilet.mission, g_toilet.stage, index);
        if (BIG_locateentryz(g_toilet.compartmentBigFile, name, 0, &offset, &size))
            return size;
    }
    return -1;
}

int GetCollisionSizeFromIndex(int index) {
    char name[256];
    int offset, size;
    if (g_toilet.compartmentBigFile) {
        sprintf(name, "%d_%d_C_c%d.cdb", g_toilet.mission, g_toilet.stage, index);
        if (BIG_locateentryz(g_toilet.compartmentBigFile, name, 0, &offset, &size))
            return size;
    }
    return -1;
}

int GetRTCSizeFromIndex(int index) {
    char name[256];
    int offset, size;
    if (g_toilet.compartmentBigFile) {
        sprintf(name, "%d_%d_Art_c%d.rtc", g_toilet.mission, g_toilet.stage, index);
        if (BIG_locateentryz(g_toilet.compartmentBigFile, name, 0, &offset, &size))
            return size;
    }
    return -1;
}

void TLT_InitResourceType(void (*init)(LevelFileContentsStruct_ *), TLTResourceID type) {
    LevelFileContentsStruct_ *entry = TLT_FindNextResourceByType(type, 0);
    while (entry) {
        init(entry);
        entry = TLT_FindNextResourceByType(type, entry);
    }
}
