// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"

extern "C" void *BIG_locateentryz(void *, const char *, int, int *, int *);
extern "C" void *BIG_locatez(void *, const char *);
bool TLT_IsFileInMVDRepository(char *);
void TLT_InsertFileInMVDRepository(char *);

void *TLT_LoadFromMVDBigFile(const char *name, int *size, bool *inRepository) {
    int offset = 0;
    BIG_locateentryz(g_toilet.mvdBigFile, name, 0, &offset, size);
    char *file = (char *)BIG_locatez(g_toilet.mvdBigFile, name);
    *inRepository = false;
    if (file) {
        *inRepository = TLT_IsFileInMVDRepository(file);
        if (!*inRepository)
            TLT_InsertFileInMVDRepository(file);
    }
    return file;
}
