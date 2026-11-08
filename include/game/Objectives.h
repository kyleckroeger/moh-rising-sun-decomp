// AI-assisted scoped reconstruction from GR8E69; see docs/Objectives.md.
#ifndef GAME_OBJECTIVES_H
#define GAME_OBJECTIVES_H

// The user message table: an array of string records, a parallel CRC table and a count.
// Member names follow what the methods do with them.
struct CUserMessageRecord {
    const char *text;
    unsigned short length;
};

class CUserMessageTable {
  public:
    CUserMessageTable();
    ~CUserMessageTable();
    void Unload();
    int Loaded() const;
    const char *GetString(int);
    int GetIndexFromCRC(unsigned int);
    const char *GetStringFromCRC(unsigned int);
    int GetStringLength(int);

    CUserMessageRecord *records;
    void *field04;
    void *field08;
    unsigned int *crcTable;
    int count;
};

// Scoped view: the message table at +1632 is all the objectives code reads.
class PopUpMessageHandler {
  public:
    unsigned char unknown_0[1632];
    CUserMessageTable messages;
};
extern PopUpMessageHandler *g_pPopUpMessageHandler;
void appendMessageToPrompt(const char *, int);
void displayPromptBuffer(short, float);

// One of ten 20-byte objective records. Names are descriptive.
struct PlayerObjective {
    const char *prompt;
    unsigned char promptLength;
    int completed;
    int bonus;
    int shown;
};

// Scoped view through +208; the complete size is not established.
class CPlayerObjectives {
  public:
    PlayerObjective objectives[10];
    int mainCount;
    int bonusCount;
    int field_d0;
    CPlayerObjectives();
    void Reset();
    void AddObjective(unsigned int, unsigned int, bool, bool, bool);
    void ShowObjective(unsigned int);
    void ChangeObjectivePrompt(unsigned int, unsigned int);
    void SetObjectiveStatus(unsigned int, bool);
    int GetObjectiveStatus(unsigned int);
    int GetNumCompletedObjectives();
    int CheckIfAllMainObjectivsAreCompleted();
    int GetNumBonusObjectives();
    int GetNumCompletedBonusObjectives();
};

// Original shared storage; this declaration does not allocate a new object.
extern CPlayerObjectives g_Objectives;

#endif
