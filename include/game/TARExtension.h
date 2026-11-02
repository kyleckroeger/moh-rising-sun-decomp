#ifndef GAME_EAGL_TAR_H
#define GAME_EAGL_TAR_H
#include "EAGLMemory.h"
#include "PlacementNew.h"
#include "EAGLDynamicLoader.h"
#include <dolphin/gx/GXTexture.h>
class SHAPE;
// The first 16 bytes are a header view, not the full variable-sized SHAPE.
struct ShapeHeaderView {
    unsigned char format;
    unsigned char unknown_1[3];
    short width, height;
    unsigned int unknown_8;
    unsigned int flags;
};
static inline const ShapeHeaderView &ShapeHeader(const SHAPE *shape) {
    return *(const ShapeHeaderView *)shape;
}
static inline const void *ShapePixels(const SHAPE *shape) {
    const char *base = (const char *)shape;
    if (ShapeHeader(shape).flags & 0x10000000)
        return base + *(const int *)(base + 16);
    return base + 16;
}
extern "C" const SHAPE *SHAPE_clut(const SHAPE *);
namespace EAGL {
enum ClampMode { CM_CLAMP, CM_WRAP, CM_MIRROR, CLAMP_MODE_0 = CM_CLAMP };
enum FilterMode { FM_POINT = 1, FM_BILINEAR = 2, FM_ANISOTROPIC = 3, FILTER_MODE_2 = FM_BILINEAR };
enum MIPMAPMode { MMM_OFF, MMM_NEAREST, MMM_LINEAR };
enum GCClampMode { GCCM_CLAMP, GCCM_WRAP, GCCM_MIRROR };
struct TexGenState {
    GXTexGenType type[8];
    GXTexGenSrc source[8];
    unsigned int matrix[8];
    unsigned char normalize[8];
    unsigned int postMatrix[8];
};
// Instance storage: 84-byte copy constructor extent; owning TAR is 88 bytes.
class TARExtension {
  public:
    const SHAPE *shape;
    int numMipmaps;
    GXTexFmt format;
    FilterMode filterMode;
    MIPMAPMode mipmapMode;
    float lodBias;
    GXTexWrapMode clampU, clampV;
    GXTexObj texture;
    GXTlutObj palette;
    unsigned char lodBiasClamp;
    unsigned char unknown_4d[3];
    bool usesPalette;
    static int gNumTexGens;
    static TexGenState gTexGens;
    static void SetNumTexGens(int);
    static void SetTexCoordGen(GXTexCoordID, GXTexGenType, GXTexGenSrc, unsigned int, unsigned char,
                               unsigned int);
    static void ResetTexCoordGens();
    TARExtension();
    ~TARExtension();
    int InitFromShape(const SHAPE *);
    bool SetClut(const SHAPE *);
    void Use(GXTexMapID, unsigned int);
    void SetTextureLODStates();
    void SetLODBiasClamp(bool);
    void SetClampModeU(GCClampMode);
    void SetClampModeV(GCClampMode);
};
class TAR {
  public:
    unsigned int unknown_0;
    TARExtension extension;
    void Use();
    bool SwapShape(const SHAPE *);
    static void operator delete(void *p, unsigned int n) { EAGLInternal::EAGLFree(p, n); }
    TAR();
    TAR(const SHAPE *);
    TAR(const TAR &);
    ~TAR();
    const SHAPE *GetShape() const;
    bool SwapClut(const SHAPE *);
    bool SetClampMode(ClampMode);
    bool SetFilterMode(FilterMode);
    bool SetMIPMAPLODBias(float);
    bool SetMIPMAPMode(MIPMAPMode);
};
int PrintMessage(int, const char *, ...);
} // namespace EAGL
namespace EAGLInternal {
extern unsigned char failed_shape[16464];
static inline const SHAPE *GetFailedShape() {
    const unsigned char *base = failed_shape;
    return (const SHAPE *)(base + ((const int *)base)[5]);
}
} // namespace EAGLInternal
#endif
