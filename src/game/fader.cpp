// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original fader.cpp. Compiled with -ffast-math: the fade from black drops its
// additions and subtractions of the known zero red channel.
#include "Fader.h"
#include "Screen.h"
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int, void *p) { return p; }

extern CFader *g_pFader;

static COORD3 Coord(float x, float y, float z) {
    COORD3 c;
    c.x = x;
    c.y = y;
    c.z = z;
    c.w = 1.0f;
    return c;
}

CFader::CFader() {
    primitive = 0;
    FaderPrimitive *p = (FaderPrimitive *)DWI_alloc("source/gfx/fader.cpp:80", sizeof(FaderPrimitive), 0x400);
    new (p) EAGL::DrawGouraud;
    primitive = p;
    p->DrawGouraud::Init();
    EAGL::GeoPrimState *state = p->GetState();
    state->SetTransparencyMethod(EAGL::TM_ALPHA);
    state->SetAlphaBlendMode(EAGL::ABM_BLEND);
    state->SetDepthTestMethod((EAGL::DepthTestMethod)7);
    state->SetAlphaTestMethod((EAGL::AlphaTestMethod)7);
    state->SetCullEnable(false);
    state->extension.SetZWritesEnable(false);
    primitive->color.Zero();
    primitive->start = 0.0f;
    primitive->duration = 0.0f;
    primitive->time = 0.0f;
}

void CFader::Init() {
    if (!g_pFader)
        g_pFader = new (DWI_alloc("source/gfx/fader.cpp:110", sizeof(CFader), 0x400)) CFader;
}

static inline void Lerp(FadeColor &out, const FadeColor &from, const FadeColor &to, float t) {
    float fg = from.g;
    float fb = from.b;
    float fa = from.a;
    float fr = from.r;
    out.r = (to.r - fr) * t + fr;
    out.g = (to.g - fg) * t + fg;
    out.b = (to.b - fb) * t + fb;
    out.a = (to.a - fa) * t + fa;
}

// Fades from black to the fade colour over start seconds, holds, then fades back over duration.
void CFader::Update(float elapsed) {
    if (g_pFader) {
        FaderPrimitive *p = g_pFader->primitive;
        FadeColor black;
        black.Zero();
        FadeColor color;
        if (!(p->time >= p->start)) {
            Lerp(color, black, p->color, p->time / p->start);
            p->time += elapsed;
        } else if (p->duration == 3.40282347e+38f) {
            color = p->color;
        } else if (!(p->time >= p->start + p->duration)) {
            Lerp(color, p->color, black, (p->time - p->start) / p->duration);
            p->time += elapsed;
        } else {
            color = black;
        }
        float r2 = color.r + color.r;
        float g2 = color.g + color.g;
        float b2 = color.b + color.b;
        float a2 = color.a + color.a;
        color.r = r2;
        color.g = g2;
        color.b = b2;
        color.a = a2;
        if (!(a2 <= 0.0f)) {
            if (!(a2 <= 255.0))
                color.a = 255.0f;
            if (!(r2 <= 255.0))
                color.r = 255.0f;
            if (!(g2 <= 255.0))
                color.g = 255.0f;
            if (!(b2 <= 255.0))
                color.b = 255.0f;
            EAGL::Colour colour;
            int a = (int)color.a;
            int r = (int)color.r;
            int g = (int)color.g;
            int b = (int)color.b;
            colour.value = (r << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (a & 0xff);
            float width = (float)CScreen::m_sWidth;
            float height = (float)CScreen::m_sHeight;
            p->Begin((EAGL::PrimitiveType)152);
            p->AddVertex(Coord(0.0f, 0.0f, 1.0f), colour);
            p->AddVertex(Coord(width, 0.0f, 1.0f), colour);
            p->AddVertex(Coord(0.0f, height, 1.0f), colour);
            p->AddVertex(Coord(width, height, 1.0f), colour);
            p->End();
            p->InternalFlush();
        }
    }
}

bool CFader::IsFading() {
    if (g_pFader) {
        FaderPrimitive *p = g_pFader->primitive;
        if (!(p->time >= p->start))
            return true;
        if (p->duration == 3.40282347e+38f)
            return false;
        return p->time < p->start + p->duration;
    }
    return false;
}

void CFader::FadeIn(CColor color, float duration) {
    if (g_pFader) {
        FaderPrimitive *p = g_pFader->primitive;
        p->GetState()->SetAlphaBlendMode(EAGL::ABM_BLEND);
        p->color = FadeColor((float)color.r, (float)color.g, (float)color.b, (float)color.a);
        p->start = 0.0f;
        p->duration = duration;
        p->time = 0.0f;
    }
}

void CFader::FadeOut(CColor color, float duration) {
    if (g_pFader) {
        FaderPrimitive *p = g_pFader->primitive;
        p->GetState()->SetAlphaBlendMode(EAGL::ABM_BLEND);
        p->color = FadeColor((float)color.r, (float)color.g, (float)color.b, (float)color.a);
        p->start = duration;
        p->duration = 3.40282347e+38f;
        p->time = 0.0f;
    }
}

void CFader::Flash(CColor color, float start, float duration) {
    if (g_pFader) {
        FaderPrimitive *p = g_pFader->primitive;
        p->GetState()->SetAlphaBlendMode(EAGL::ABM_ADD);
        p->color = FadeColor((float)color.r, (float)color.g, (float)color.b, (float)color.a);
        p->start = start;
        p->duration = duration;
        p->time = 0.0f;
    }
}
