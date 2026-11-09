// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original HeadsUpDisplay.cpp. See hud_stamp's XFiles note.
#include "Font.h"
struct XFilesState {
    char text[256];
    float alpha;                  // +0x100
    float frame;
    int landmarkMarker;
    int timestampMarker;
    int displayLandmark;
    int displayTimestamp;
    int doneParsingLandmark;
    int doneParsingTimestamp;
    int doneFading;               // +0x120
    int doneShowing;              // +0x124
};
extern XFilesState XFiles;
extern CFont *timestampFont;

void FadeTheXFilesText(float elapsed) {
    XFiles.alpha -= elapsed * 0.350350380f;
    if (!(XFiles.alpha >= 0.0f)) {
        XFiles.doneFading = 1;
        XFiles.doneShowing = 1;
    }
    timestampFont->SetColor(CColor(0x80, 0x80, 0x80, (int)XFiles.alpha));
    if (XFiles.alpha == 0.0f) {
        XFiles.alpha = 128.0f;
        XFiles.doneFading = 0;
        XFiles.doneShowing = 0;
    }
}

class CHeadsUpDisplay {
public:
    void ChangeHitStatus(unsigned int direction);
    void AddHitDirection(unsigned int direction);
};

void CHeadsUpDisplay::AddHitDirection(unsigned int direction) {
    ChangeHitStatus(direction);
}
