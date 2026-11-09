// A fragment of EA's MAD movie codec (rcmp_mad_codec.cpp): a decoded frame's shape storage.
// Names come from the symbol table; members are inferred from the code.
#include "Rcmp.h"
extern "C" void MEM_fill(void *, int, int);

struct MAD_SHAPE {
    unsigned char kind;
    unsigned char unknown01[3];
    short width;  // +0x04
    short height; // +0x06
};

class MAD_FRAME {
  public:
    int unknown00;
    MAD_SHAPE *shape;
    int unknown08;
    int unknown0c;
    int unknown10;
    MAD_FRAME(unsigned int, unsigned int);
    ~MAD_FRAME();
    static void operator delete(void *p) { RCMP::rcmp_sys.release(p); }
};

MAD_FRAME::MAD_FRAME(unsigned int width, unsigned int height) {
    unknown08 = 3;
    unknown10 = 0;
    unknown0c = 3;
    unknown00 = 1;
    shape = (MAD_SHAPE *)RCMP::rcmp_sys.allocate("MAD_FRAME::m_Shape", (width * ((height << 1) + height) >> 1) + 16, 0, 16, (int)RCMP::rcmp_sys.field8);
    MEM_fill(shape, 0, 16);
    shape->kind = 108;
    shape->width = width;
    shape->height = height;
}

MAD_FRAME::~MAD_FRAME() {
    if (shape) {
        RCMP::rcmp_sys.release(shape);
        shape = 0;
    }
    unknown08 = 7;
    unknown0c = 7;
}
