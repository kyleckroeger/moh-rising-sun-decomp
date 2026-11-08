#ifndef GAME_TOILET_H
#define GAME_TOILET_H

// Toilet is the game's file loader. The class, function and global names come from the symbol
// table. Members are inferred from how the reconstructed loader functions use them, and only the
// members those functions reach are named.
enum TLTResourceID { TLTResourceUnknown = 0 };

struct TLTResourceEntry {
    int unknown00;
    int unknown04;
    int value; // +0x08
};

// A level-contents entry: 12 bytes, a type and a table of value records.
struct LevelFileContentsStruct_ {
    TLTResourceID type;
    int unknown04;
    TLTResourceEntry **data; // +0x08
};

class CRtcFile {
public:
    void DeallocateArchiveBuffer();
    unsigned char unknown[0x10];
};

class Toilet {
public:
    int mission;                           // +0x000
    int stage;                             // +0x004
    int asyncState;                        // +0x008
    float asyncPercent;                    // +0x00c
    int loadFileNormalLocked;              // +0x010
    int asyncRequest;                      // +0x014
    char asyncFilename[0x100];             // +0x018
    char *levelBigFile;                    // +0x118
    int levelBigFileSize;                  // +0x11c
    char *bigFile120;                      // +0x120
    int bigFile120Size;                    // +0x124
    char *pauseBigFile;                    // +0x128
    int pauseBigFileSize;                  // +0x12c
    void *compartmentBigFile;              // +0x130
    unsigned char unknown134[0x160 - 0x134];
    LevelFileContentsStruct_ *contents;    // +0x160
    LevelFileContentsStruct_ *contentsEnd; // +0x164
    unsigned char unknown168[0x184 - 0x168];
    CRtcFile rtcFile;                      // +0x184
    int asyncStateBase;                    // +0x194
};
extern Toilet g_toilet;

LevelFileContentsStruct_ *TLT_FindNextResourceByType(TLTResourceID, LevelFileContentsStruct_ *);
LevelFileContentsStruct_ *TLT_FindNextResourceByTypeAndVal(TLTResourceID, int, int, LevelFileContentsStruct_ *);
void *TLT_LoadFileFromBigFile(char *, const char *, int *);
void *TLT_LoadFileFromLevelBigFile(const char *, int *);
void GetCompartmentNameFromIndex(int, char *, int);

#endif
