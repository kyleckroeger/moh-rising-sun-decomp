// AI-assisted reconstruction; the header name is a project choice. The queued subtitle record of the
// original pumhandler.cpp; its members are inferred from their use.
#ifndef GAME_SUBTITLEQUEUE_H
#define GAME_SUBTITLEQUEUE_H
#include "Color.h"
#include "DwiList.h"

struct QueuedSubtitle {
    QueuedSubtitle() : unknown00(0.0f), delay(0.0f), shown(0) {
        CColor *c = &color;
        c->r = 0;
        c->g = 0;
        c->b = 0;
        c->a = 255;
    }
    float unknown00;
    float delay;  // +0x04
    float time;   // +0x08
    int shown;    // +0x0c
    CColor color; // +0x10
    char text[60]; // +0x14
};
extern dwi::list<QueuedSubtitle> g_subtitle_queue;
extern int g_subtitle_queue_initialized;
#endif
