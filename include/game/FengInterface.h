// CFengInterface: an FEng game interface drawing with EAGL. Scoped view; see docs/Frontend.md.
#ifndef GAME_FENG_INTERFACE_H
#define GAME_FENG_INTERFACE_H
#pragma interface
#include "FEngMath.h"
#include "FEngObjects.h"
#include "FEColor.h"
#include <stdio.h>
#include <string.h>
extern "C" void *DWI_alloc(const char *, int, int);
struct ShapeFile;
namespace EAGL {
class Transform {
  public:
    float m[16];
    void BuildIdentity();
    void BuildScale(float, float, float, float);
    void AppendTranslate(float, float, float);
    void PostMult(const Transform &);
};
}
class CSprite {
  public:
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
    CSprite(ShapeFile *);
    ~CSprite();
    unsigned char unknown_0[36];
};
class CFont {
  public:
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
    CFont(char *);
    ~CFont();
    unsigned char unknown_0[20];
};
// Object kinds by what the render switch does with each. The names are inferred; the range is what
// the switch's comparison tree shows: it leaves out the last value's test, so the type ends at 12.
enum FEObjectKind {
    FEObjectKindInvalid = -1,
    FEObjectKindNone,
    FEObjectKindImage,
    FEObjectKindString,
    FEObjectKindModel,
    FEObjectKindList,
    FEObjectKindGroup,
    FEObjectKindCodeList,
    FEObjectKindMovie,
    FEObjectKindEffect,
    FEObjectKindColoredImage,
    FEObjectKindUnknown10,
    FEObjectKindSimpleImage,
    FEObjectKindCount
};
class FEImage;
class FESimpleImage;
class FEString;
// A color as four floats from 0 to 1. Name inferred.
struct SFengColor {
    float a, r, g, b;
    SFengColor() { b = g = r = a = 0.0f; }
    void operator=(const FEColor &color) {
        int red = color.r;
        int green = color.g;
        int blue = color.b;
        int alpha = color.a;
        r = red * (1.0f / 255.0f);
        g = green * (1.0f / 255.0f);
        b = blue * (1.0f / 255.0f);
        a = alpha * (1.0f / 255.0f);
    }
    void operator*=(SFengColor other) {
        a *= other.a;
        r *= other.r;
        g *= other.g;
        b *= other.b;
    }
};
// One object's drawing state, indexed by its render context number. Name inferred.
struct SFengRenderContext {
    EAGL::Transform transform;  // +0x00
    SFengColor color;           // +0x40
    unsigned char unknown_50[16];
};
class CFengInterface : public FEGameInterface {
  public:
    void SetBigFilePtr(char *);
    int GetPackageData(const char *, unsigned char **, bool &);
    int LocalizePackage(FEPackage *);
    int LoadResources(FEPackage *, long, FEResourceRequest *);
    int UnloadResources(FEPackage *, long, FEResourceRequest *);
    void PackageWasLoaded(FEPackage *);
    int PackageWillUnload(FEPackage *);
    void BeginPackageRendering(FEPackage *);
    void EndPackageRendering(FEPackage *);
    void GenerateRenderContext(unsigned short, FEObject *);
    int GetContextTransform(unsigned short, FEMatrix4 &);
    void GetViewTransformation(FEMatrix4 *);
    void RenderObject(FEObject *);
    unsigned long GetJoyPadMask(unsigned char);
    void ReplaceFilenameExtension(char *, char *);
    int LoadTexture(FEResourceRequest *, char *);
    int LoadFont(FEResourceRequest *, char *);
    void GenerateXformMatrix(EAGL::Transform *, FEObjData *, unsigned short);
    void RenderImage(FEImage *);
    void Render(FESimpleImage *);
    void RenderString(FEString *);
    void DebugMessageQueued(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
    void DebugMessageProcessed(unsigned long, FEObject *, FEObject *, FEPackage *, unsigned long);
    void GetMouseInfo(FEMouseInfo &);
    unsigned char unknown_4[0x60 - 4];
    SFengRenderContext contexts[64]; // +0x60
    char *bigFile;                   // +0x1860
    FEngine *engine;                 // +0x1864
    int lastStickX;                  // +0x1868; the stick's last value in GetJoyPadMask.
    int lastStickY;                  // +0x186c
    int unknown_1870;                // +0x1870; cleared by UnloadResources.
};
#endif
