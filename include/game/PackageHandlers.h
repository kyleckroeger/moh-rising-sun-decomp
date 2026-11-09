// The shell's package handlers; scoped views, see docs/Frontend.md.
#ifndef GAME_PACKAGE_HANDLERS_H
#define GAME_PACKAGE_HANDLERS_H
#pragma interface
#include "FEngObjects.h"
#include "FELocalizer.h"
#include "MemoryCard.h"
#include "VolumeKnobs.h"
class ShellFactory {
  public:
    PackageHandler *CreatePackageHandler(const char *);
    void DestroyPackageHandler(PackageHandler *);
};
// The second table of the handlers that scan memory cards.
struct CMemoryCard::ScanTask {
    virtual ~ScanTask();
    virtual void Execute(const char *, unsigned int);
};
class HandlerVirtualKeyboard : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int HandleSoundEvent(unsigned int, FEObject *);
    virtual char *GetMyString();
    virtual int GetStringLength();
    void ClearStringField();
    void AddLetterToString(char *, char);
    void RemovePreviousLetter(char *);
};
class HandlerSaveSlotBase : public PackageHandler {
  public:
    HandlerSaveSlotBase();
    ~HandlerSaveSlotBase();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int HandleSoundEvent(unsigned int, FEObject *);
    virtual void OnCancel();
    virtual void OnAccept();
    virtual void OnPush();
    virtual void OnPop();
    int GetSelection() const;
    int IsValidLoadGameType(int);
    static void Push(const char *);
    static void Swap(const char *, FEPackage *);
    void Pop();
};
class HandlerControllerOptions : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int crosshairs;         // +0x50
    int vibrate;            // +0x54
    int grenadeCook;        // +0x58
    int invertY;            // +0x5c
    float analogSensitivity; // +0x60
    float turnSpeed;        // +0x64
    CSlider sensitivitySlider; // +0x68
    CSlider turnSpeedSlider;   // +0x88
    int player;             // +0xa8
};
class HandlerAudioConfig : public PackageHandler {
  public:
    HandlerAudioConfig() { confirmed = 1; }
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void Revert();
    float savedSfxVolume;   // +0x50
    float savedMusicVolume; // +0x54
    int soundFormat;        // +0x58
    int subtitles;          // +0x5c
    CVolumeKnobs knobs;     // +0x60
    int confirmed;          // +0xa0; when clear, setting the screen up reverts it.
};
class HandlerCoopProfile : public PackageHandler, public CMemoryCard::ScanTask {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_54[0x5c - 0x54];
};
class HandlerProfileSelect : public PackageHandler {
  public:
    HandlerProfileSelect();
    static int InSelection();
    static int InKeyboard();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[4212 - 0x50];
};
class HandlerMPProfile : public PackageHandler {
  public:
    HandlerMPProfile();
    static int InSelection();
    static int InKeyboard();
    static int m_lastProfile;
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[100 - 0x50];
};
class HandlerQuadrantGroup : public PackageHandler {
  public:
    int Init(const char *);
};
class PackageHandlerMain : public PackageHandler {
  public:
    void HandleTick();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerNumPlayers : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerGameOptions : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerNameEntry : public HandlerVirtualKeyboard {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    char *GetMyString();
    int GetStringLength();
    int cursor;    // +0x50
    char text[12]; // +0x54
};
class HandlerDifficulty : public PackageHandler {
  public:
    HandlerDifficulty();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned long buttonHashes[3]; // +0x50; hashes of the three difficulty buttons' names.
};
class HandlerCustomController : public PackageHandler {
  public:
    HandlerCustomController();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[112 - 0x50];
};
class HandlerSplitScreen : public PackageHandler {
  public:
    ~HandlerSplitScreen();
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    virtual void GotoNextScreen();
    unsigned char unknown_50[0x5c - 0x50];
    int pendingPlayerMask; // +0x5c
};
class HandlerMPGameOptions : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[92 - 0x50];
};
class HandlerMPGameGlobalOptions : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[96 - 0x50];
};
class HandlerMPJoin : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMPWaiting : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    static int mbSplitScreenContinueSend;
};
class HandlerMPBots : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[160 - 0x50];
};
class HandlerMPBotChild : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[96 - 0x50];
};
class HandlerLevelMenu : public PackageHandler {
  public:
    HandlerLevelMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[84 - 0x50];
};
class HandlerMainMenu : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerControllerLayout : public PackageHandler {
  public:
    HandlerControllerLayout();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[112 - 0x50];
};
class HandlerCoopTempLaunch : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerSpecialFeatures : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMovieMenu : public PackageHandler {
  public:
    HandlerMovieMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[164 - 0x50];
};
class HandlerCoop4WaySplit : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    virtual void GotoNextScreen();
    int playerCount; // +0x50
    unsigned char unknown_54[96 - 0x54];
};
class HandlerCoopGameOptions : public PackageHandler {
  public:
    static int mSaveSlot;
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void SetOptionsProperties();
    FEObject *mapNumberText; // +0x50
};
class HandlerLoadGame : public HandlerSaveSlotBase {
  public:
    HandlerLoadGame();
    ~HandlerLoadGame();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void OnAccept();
};
class HandlerBonusOptions : public PackageHandler {
  public:
    HandlerBonusOptions();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    bool GetCheatByIndex(int);
    void SetCheatByIndex(int, bool);
    bool ToggleCheatByIndex(int);
    unsigned long textHashes[12]; // +0x50
};

class HandlerStatsScreen : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void CheckToSeeIfSpecialPearlStatsAreNeeded(bool);
    int specialPearlStats; // +0x50
};
class HandlerMapOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMedalsOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerPasswordEntry : public HandlerVirtualKeyboard {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[104 - 0x50];
};
class HandlerLanguageMenu : public PackageHandler {
  public:
    HandlerLanguageMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int selected;     // +0x50; 1-based, as SetObjectListIndex takes it.
    int languages[6]; // +0x54; [0] is unused, the constructor fills 1 to 5 and HandleEvent reads from 1.
};
class HandlerOnlineOptions : public PackageHandler {
  public:
    HandlerOnlineOptions();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[204 - 0x50];
};
class HandlerSaveProfile : public PackageHandler {
  public:
    HandlerSaveProfile();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[92 - 0x50];
};
class HandlerOnlineKeyboard : public HandlerVirtualKeyboard {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[104 - 0x50];
};
class HandlerOnlineFavorites : public PackageHandler {
  public:
    HandlerOnlineFavorites();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[88 - 0x50];
};
class HandlerOnlineNetConfWarning : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerHUDPreferences : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[92 - 0x50];
};
class HandlerPausePreferences : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class CSaveGamePopup {
  public:
    static int IsBackgroundEnabled();
    static void EnableBackground();
    static void DisableBackground();
};
class HandlerPauseOutro : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerPauseOutroCoop : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerPauseBriefingOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int isHistory; // +0x50; which of the two screens is showing.
};
class HandlerSaveSlotSelect : public HandlerSaveSlotBase {
  public:
    HandlerSaveSlotSelect();
    ~HandlerSaveSlotSelect();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    static void Push();
    void OnCancel();
    void OnAccept();
    void OnPop();
};
class HandlerLoadSlotSelect : public HandlerSaveSlotBase {
  public:
    HandlerLoadSlotSelect();
    ~HandlerLoadSlotSelect();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void OnCancel();
    void OnAccept();
};
class HandlerLeaderboard : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void fillNumString(const char *, int, bool);
    void fillNumGroupString(FEGroup *, const char *, int, bool);
    unsigned char unknown_50[84 - 0x50];
};
class Handler_2P_Menu : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
#endif
