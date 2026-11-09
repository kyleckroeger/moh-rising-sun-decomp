// AI-assisted reconstruction from GR8E69. The image records no name for this file. Class and
// method names come from the symbol table.
#include "Shell.h"

extern "C" int PAD_active(int pad);

const void *CGcDevice::GetInputState() const {
    if (port == -1)
        return 0;
    if (PAD_active(port))
        return (const char *)this + 2;
    return 0;
}

CInputEvent *CGcDevice::GetEventTable() {
    if (port == -1)
        return 0;
    return (CInputEvent *)((char *)this + 0x60);
}

const CInputEvent *CGcDevice::GetEventTable() const {
    if (port == -1)
        return 0;
    return (const CInputEvent *)((const char *)this + 0x60);
}
