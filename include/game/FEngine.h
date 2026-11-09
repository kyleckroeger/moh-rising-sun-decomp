// FEngine: FEng's root object. Scoped view; see docs/Frontend.md.
#ifndef GAME_FENGINE_H
#define GAME_FENGINE_H
#include "FEngLists.h"

class FEngine {
  public:
    void Render();
    unsigned long GetCombinedJoyPadActiveMask() const;
    void SetNumJoyPads(unsigned char);
    void ResetJoyPad();
    void SetExecution(bool);
    void AddToIdleList(FEPackage *);
    FEPackage *FindIdlePackage(const char *) const;
    FEPackage *GetFirstLibrary() const;
    void AddToLibraryList(FEPackage *);
    void RemoveFromLibraryList(FEPackage *);
    FEPackage *FindLibraryPackage(unsigned long) const;
    FEObject *FindObject(unsigned long, const char *) const;
    FEObject *FindObject(const char *, const char *) const;
    void StartCheckMovieDemoScreen();
    void QueueMessage(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
    void QueuePackagePush(const char *, FEPackage *, unsigned long);
    void QueuePackageSwitch(const char *, FEPackage *);
    void RecordLastPackageButton(unsigned long, unsigned long);
    unsigned long RecallLastPackageButton(unsigned long);
    int RecordPackageMarker(const char *);
    char *RecallPackageMarker();
    void ClearPackageMarkers();

    int checkMovieDemoScreen;    // +0x0000
    int executing;               // +0x0004
    unsigned char unknown_8[0x4c - 8];
    unsigned char joyPadState[76]; // +0x004c; cleared as a block by ResetJoyPad.
    unsigned char unknown_98[0xe8 - 0x98];
    int joyPadCount;             // +0x00e8
    unsigned short activeMask;   // +0x00ec
    unsigned char unknown_ee[0xf0 - 0xee];
    FEList packages;             // +0x00f0
    FEList idlePackages;         // +0x0100
    FEList libraries;            // +0x0110
    unsigned char unknown_120[0x415c - 0x120];
    int markerCount;             // +0x415c
    char markers[16][256];       // +0x4160
    int lastButtonIndex;         // +0x5160
    struct { unsigned long package; unsigned long button; } lastButtons[32]; // +0x5164
    unsigned char unknown_5264[8];
    signed char lastAcceptPad;   // +0x526c; read by ShellManager::GetIndexPadLastPressedAccept.
};
#endif
