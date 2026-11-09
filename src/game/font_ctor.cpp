// CFont's constructor and destructor.
#include "Font.h"
extern FONT *FONTstand;

CFont::CFont(char *name) {
    handle = CreateHandle(name);
    handle->field48 = 1;
    handle->field4c = 0;
    justification = 0;
    snapY = 0;
    snapX = 0;
}

CFont::~CFont() {
    if (handle != FONTstand)
        DestroyHandle(handle);
    handle = 0;
}
