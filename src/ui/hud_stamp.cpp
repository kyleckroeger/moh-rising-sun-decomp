// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original HeadsUpDisplay.cpp. timestampFont and XFiles come from the symbol table.
// The members of XFiles are inferred from the Frontline counterparts' file-local variables.
#include "Font.h"
#include "Screen.h"
#include "Toilet.h"
#include <string.h>

extern "C" void *DWI_alloc(const char *, int, int);
void DisplayLandmarkStamp(float);
void DisplayTimeStamp(float);
void FadeTheXFilesText(float);
void ResetTimeStampStuff();


inline void *operator new(unsigned int, void *p) { return p; }

struct XFilesState {
    char landmark[64];            // +0x000
    char timestamp[64];           // +0x040
    char landmarkDisplayText[64]; // +0x080
    char timestampDisplayText[64]; // +0x0c0
    float alpha;                  // +0x100
    float frame;                  // +0x104
    int landmarkMarker;           // +0x108
    int timestampMarker;          // +0x10c
    int displayLandmark;          // +0x110
    int displayTimestamp;         // +0x114
    int doneParsingLandmark;      // +0x118
    int doneParsingTimestamp;     // +0x11c
    int doneFading;               // +0x120
    int doneShowing;              // +0x124
};
extern XFilesState XFiles;
extern CFont *timestampFont;

void InitTimeStampStuff() {
    LevelFileContentsStruct_ *resource = TLT_FindNextResourceByType((TLTResourceID)25, 0);
    char *file = (char *)TLT_LoadFileFromLevelBigFile((const char *)resource->data[2]->value, 0);
    timestampFont = new (DWI_alloc("source/UserInterface/HeadsUpDisplay.cpp:3237", sizeof(CFont), 0x400)) CFont(file);
    ResetTimeStampStuff();
}

void ResetTimeStampStuff() {
    if (timestampFont) {
        timestampFont->SetColor(CColor(0x80, 0x80, 0x80, 0x80));
        timestampFont->SetScale(CScreen::m_sWidthScale, CScreen::m_sHeightScale);
        if (CScreen::m_spScreen)
            timestampFont->SetDepth(CScreen::m_spScreen->GetDepthBufferMax());
    }
    XFiles.frame = 0.0f;
    memset(XFiles.landmark, 0, 64);
    memset(XFiles.timestamp, 0, 64);
    memset(XFiles.landmarkDisplayText, 0, 64);
    memset(XFiles.timestampDisplayText, 0, 64);
    XFiles.alpha = 128.0f;
    XFiles.landmarkMarker = 0;
    XFiles.timestampMarker = 0;
    XFiles.displayLandmark = 0;
    XFiles.displayTimestamp = 0;
    XFiles.doneParsingLandmark = 0;
    XFiles.doneParsingTimestamp = 0;
    XFiles.doneFading = 0;
    XFiles.doneShowing = 0;
}

void SetLandmarkStampString(const char *text) {
    if (text) {
        strncpy(XFiles.landmark, text, 63);
        XFiles.landmark[63] = 0;
        XFiles.displayLandmark = 1;
        if (strcmp(text, " ") == 0) {
            XFiles.landmark[0] = 0;
            XFiles.doneParsingLandmark = 1;
        }
    } else {
        XFiles.landmark[0] = 0;
        XFiles.doneParsingLandmark = 1;
    }
}

void SetTimeStampString(const char *text) {
    if (text) {
        strncpy(XFiles.timestamp, text, 63);
        XFiles.timestamp[63] = 0;
        if (strcmp(text, " ") == 0) {
            XFiles.timestamp[0] = 0;
            XFiles.doneParsingTimestamp = 1;
            XFiles.doneFading = 1;
            XFiles.doneShowing = 1;
        }
    } else {
        XFiles.timestamp[0] = 0;
        XFiles.doneParsingTimestamp = 1;
        XFiles.doneFading = 1;
        XFiles.doneShowing = 1;
    }
}

void UpdateXFilesText(float elapsed) {
    if (!XFiles.doneShowing) {
        if (XFiles.displayLandmark)
            DisplayLandmarkStamp(elapsed);
        if (XFiles.displayTimestamp)
            DisplayTimeStamp(elapsed);
        if (XFiles.doneParsingTimestamp && !XFiles.doneFading)
            FadeTheXFilesText(elapsed);
    }
}
