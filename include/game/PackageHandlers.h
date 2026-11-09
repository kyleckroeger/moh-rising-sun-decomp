// The shell's package handlers; scoped views, see docs/Frontend.md.
#ifndef GAME_PACKAGE_HANDLERS_H
#define GAME_PACKAGE_HANDLERS_H
#pragma interface
#include "FEngObjects.h"
#include "FELocalizer.h"
class HandlerDifficulty : public PackageHandler {
  public:
    HandlerDifficulty();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned long buttonHashes[3]; // +0x50; hashes of the three difficulty buttons' names.
};
class HandlerMPJoin : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerCoopTempLaunch : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMedalsOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerQuadrantGroup : public PackageHandler {
  public:
    int Init(const char *);
};
class HandlerNumPlayers : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class PackageHandlerMain : public PackageHandler {
  public:
    void HandleTick();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerProfileSelect : public PackageHandler {
  public:
    static int InSelection();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMPProfile : public PackageHandler {
  public:
    static int InSelection();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerLanguageMenu : public PackageHandler {
  public:
    HandlerLanguageMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int selected;     // +0x50; 1-based, as SetObjectListIndex takes it.
    int languages[6]; // +0x54; [0] is unused, the constructor fills 1 to 5 and HandleEvent reads from 1.
};
#endif
