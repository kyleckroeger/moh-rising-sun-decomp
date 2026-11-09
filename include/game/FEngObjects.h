// FEng objects and package handlers; scoped views, see docs/Frontend.md.
#ifndef GAME_FENG_OBJECTS_H
#define GAME_FENG_OBJECTS_H
#include "FEngLists.h"
#include "FEngStrings.h"
#include "ShellManager.h"
class FEScriptView {
  public:
    unsigned char unknown_0[0x2c];
    const char *name; // +0x2c
};

// Scoped view of the object fields the handlers change.
class FEStringObject : public FEObject {
  public:
    unsigned char unknown_14[0x18 - 0x14];
    int type;                     // +0x18; 5 for a group, 2 for a string in SetGroupString.
    unsigned int unknownBits : 5; // +0x1c, the object's flag word.
    unsigned int disabled : 1;    // Set by DisableButton.
    unsigned int unknownBit6 : 3;
    unsigned int textChanged : 1; // Set whenever the text is replaced.
    unsigned char unknown_20[0x54 - 0x20];
    FEScriptView *currentScript;  // +0x54
    unsigned char unknown_58[0x60 - 0x58];
    FEWideString text;            // +0x60
};

class PackageHandler {
  public:
    virtual ~PackageHandler();
    int Init(const char *);
    int SetScript(FEPackage *, const char *, const char *);
    int SetScript(FEPackage *, unsigned long, const char *);
    int GetCurrentLanguage();
    int SetObjectListIndex(char *, unsigned int);
    int SetNextObjectListIndex(char *);
    int SetPrevObjectListIndex(char *);
    int GetObjectListIndex(char *);
    void SetProfileNameOnScreen(int);
    void Set2ProfileNamesOnScreen();
    int SetGroupString(FEGroup *, const char *, char *);
    int SetGroupString(unsigned int, unsigned int, FEWideString *);
    int SetGroupString(unsigned int, unsigned int, const char *);
    int SetGroupShowOnly(FEGroup *, unsigned long);
    int SetGroupShowOnly(FEGroup *, const char *);
    int SetGroupShowOnly(unsigned long, unsigned long);
    void QueueMessage(unsigned int);
    void HandleTick();
    class ShellManager *shell; // +0x00
    class FEngine *engine;     // +0x04
    FEPackage *package;        // +0x08
    char name[0x40];           // +0x0c
    void SetObjectString(FEPackage *, const char *, const char *);
    void SetObjectString(FEPackage *, const char *, FEWideString *);
    void SetObjectString(FEPackage *, const char *, const wchar_t unsigned *);
    void EnableButton(FEObject *);
    void DisableButton(FEObject *);
    void EnableButton(FEPackage *, const char *);
    void DisableButton(FEPackage *, const char *);
    void EnableButton(FEPackage *, unsigned long);
    void DisableButton(FEPackage *, unsigned long);
};
#endif
