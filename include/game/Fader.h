#ifndef GAME_FADER_H
#define GAME_FADER_H

// Class and method names come from the symbol table. The fade colour and member names are inferred
// from use.
#include "Color.h"

// A view of the EAGL render state calls used here; EAGLGeoPrimState.h holds the full class and
// needs the Dolphin SDK headers.
struct COORD3;
namespace EAGL {
struct Colour;
enum PrimitiveType { PrimitiveTypeUnknown = 0 };
enum DepthTestMethod { DTM_LEQUAL = 3 };
enum AlphaBlendMode { ABM_OFF, ABM_BLEND, ABM_ADD, ABM_ATTENUATE, ABM_MODULATE, ABM_SUBTRACT, ABM_CUSTOM };
enum AlphaTestMethod { ATM_GREATER = 4 };
enum TransparencyMethod { TM_OPAQUE, TM_ALPHA, TM_CHROMAKEY };
class GeoPrimStateExtension {
public:
    void SetZWritesEnable(bool);
};
class GeoPrimState {
public:
    GeoPrimStateExtension extension;
    bool SetCullEnable(bool);
    bool SetDepthTestMethod(DepthTestMethod);
    bool SetAlphaBlendMode(AlphaBlendMode);
    bool SetAlphaTestMethod(AlphaTestMethod);
    bool SetTransparencyMethod(TransparencyMethod);
};
class DrawGouraud {
public:
    DrawGouraud();
    void Init();
    GeoPrimState *GetState();
    void Begin(PrimitiveType type);
    void AddVertex(const COORD3 &position, const Colour &colour);
    void End();
    void InternalFlush();
    unsigned char unknown00[208];
};
}

struct COORD3 {
    float x, y, z, w;
};

namespace EAGL {
struct Colour {
    unsigned int value;
};
}

struct FadeColor {
    float r, g, b, a;
    FadeColor() {}
    FadeColor(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
    FadeColor(const FadeColor &o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
    FadeColor operator=(const FadeColor &o) {
        r = o.r;
        g = o.g;
        b = o.b;
        a = o.a;
        return *this;
    }
    void Zero() { r = g = b = a = 0.0f; }
};

struct FaderPrimitive : public EAGL::DrawGouraud {
    FadeColor color;  // +0xd0
    float start;      // +0xe0
    float duration;   // +0xe4
    float time;       // +0xe8
    int unknownEC;
};

class CFader {
public:
    CFader();
    static void Init();
    static void Update(float elapsed);
    static bool IsFading();
    static void FadeIn(CColor color, float duration);
    static void FadeOut(CColor color, float duration);
    static void Flash(CColor color, float start, float duration);

    FaderPrimitive *primitive;
};
#endif
