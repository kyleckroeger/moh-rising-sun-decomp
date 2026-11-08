// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original pumhandler.cpp. UpdateSubtitles counts down each queued subtitle's delay,
// shows it as a pop-up when the delay runs out, and removes it when its time is up. The queued
// subtitle's members are inferred from this use.
#include "Objectives.h"
#include "Color.h"
#include "DwiList.h"

struct QueuedSubtitle {
    int unknown00;
    float delay;  // +0x04
    float time;   // +0x08
    int shown;    // +0x0c
    CColor color; // +0x10
    char text[52]; // +0x14
};

void PrintSubtitle(char *, float, CColor, PopUpMessageHandler *);
void StartupSubtitles();
extern dwi::list<QueuedSubtitle> g_subtitle_queue;
extern int g_subtitle_queue_initialized;
// Only the word read here is declared.
struct PauseScreenView {
    unsigned char unknown00[0x10];
    int paused; // +0x10
};
extern PauseScreenView g_PauseScreen;

void UpdateSubtitles(float elapsed) {
    if (!g_PauseScreen.paused) {
        float step = elapsed * 0.0166833512f;
        dwi::list<QueuedSubtitle>::iterator it(0);
        if (!g_subtitle_queue_initialized)
            StartupSubtitles();
        for (it = g_subtitle_queue.begin(); it != g_subtitle_queue.end();) {
            QueuedSubtitle &subtitle = *it;
            if (!subtitle.shown) {
                if (!(subtitle.delay >= step)) {
                    subtitle.time -= step - subtitle.delay;
                    (*it).delay = 0.0f;
                    PrintSubtitle((*it).text, (*it).time, (*it).color, g_pPopUpMessageHandler);
                    (*it).shown = 1;
                } else {
                    subtitle.delay -= step;
                }
                ++it;
            } else {
                subtitle.time -= step;
                if (!((*it).time >= 0.0f))
                    it = g_subtitle_queue.erase(it);
                else
                    ++it;
            }
        }
    }
}
