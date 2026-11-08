#ifndef GAME_MOHLOADER_H
#define GAME_MOHLOADER_H
// The loader classes. Class, method and global names come from the symbol table. Members and the
// persistent block's layout are inferred from use. The pure virtual slots' names come from
// MohLoaderGc's overrides in the table. MohLoaderGc's destructor is compiler-emitted far from the
// file's other functions, so it is declared rather than defined inline.
struct LoaderPersistent {
    unsigned int gameDataMagic; // +0x00
    unsigned int returningMagic; // +0x04
    int resetButtonPressed;     // +0x08
    unsigned char gameData[1];  // +0x0c
};
extern LoaderPersistent *g_pPersistent;

class MohLoader {
public:
    MohLoader(bool valid);
    static MohLoader *Instance();
    static void *GameData();
    virtual void Initialize() = 0;
    virtual void InitVideo() = 0;
    virtual void LoadModule(const char *name) = 0;
    virtual void ValidateGameData() = 0;
    virtual ~MohLoader();
    virtual void InvalidateGameData() = 0;

    int tvFormat;                // +0x00
    unsigned char unknown04[0x54 - 4];
    bool gameDataValid;          // +0x54
    int unknown58;               // +0x58
    int resetEnabled;            // +0x5c
};

class MohLoaderGc : public MohLoader {
public:
    MohLoaderGc(bool valid);
    static void SetReturningFlag();
    static void ClearReturningFlag();
    static MohLoaderGc *InstanceGc();
    static void *GameDataGc();
    virtual void Initialize();
    virtual void InitVideo();
    virtual void LoadModule(const char *name);
    void LoadModule(const void *image, unsigned long size);
    virtual void ValidateGameData();
    virtual ~MohLoaderGc();
    virtual void InvalidateGameData();
    void RequestReset();
    int WasResetButtonPressed();
    void ResetButtonPressed(bool pressed);
    bool IsResetConsoleRequested();
    void ResetConsole();
    void CheckForControllerReset();

    int resetRequested;          // +0x64
    static MohLoaderGc *m_pLoaderGc;
};
#endif
