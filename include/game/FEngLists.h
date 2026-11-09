// FEng list and node classes; see docs/Frontend.md. Class tags and methods are from symbols;
// member names describe what the code does with them.
#ifndef GAME_FENG_LISTS_H
#define GAME_FENG_LISTS_H

class FEMinNode {
  public:
    FEMinNode *next; // +0x0
    FEMinNode *prev; // +0x4
    virtual ~FEMinNode(); // table at +0x8
};

class FENode : public FEMinNode {
  public:
    char *name;         // +0xc
    unsigned long hash; // +0x10
};

class FEMinList {
  public:
    int count;       // +0x0
    FEMinNode *head; // +0x4
    FEMinNode *tail; // +0x8
    virtual ~FEMinList(); // table at +0xc
    void AddTail(FEMinNode *node) { AddNode(tail, node); }
    void AddNode(FEMinNode *, FEMinNode *);
    FEMinNode *RemNode(FEMinNode *);
    FEMinNode *RemHead();
    FEMinNode *FindNode(unsigned long) const;
};

class FEList : public FEMinList {
  public:
    FENode *FindNode(const char *, FENode *) const;
    FENode *FindNode(const char *) const;
};

// A list that can instead refer to another list's nodes.
class FERefList {
  public:
    int isReference;  // +0x0
    FEMinNode *head;  // +0x4; the referenced FERefList when isReference is set.
    FEMinNode *tail;  // +0x8
    void ReferenceList(FERefList *);
    void AddNode(FEMinNode *, FEMinNode *);
    FEMinNode *RemNode(FEMinNode *);
    FEMinNode *RemHead();
    int GetNumElements();
    FEMinNode *GetHead() const;
};

class FEWideString;
class FEScript;
class FEMessageResponse;
class FEObject : public FENode {
  public:
    FEScript *FindScript(unsigned long) const;
    void SetCurrentScript(FEScript *);
    void SetupMoveToTracks();
    void SetDataSize(unsigned long);
    FEMessageResponse *FindResponse(unsigned long) const;
    void SetScript(unsigned long, bool);
    void SetScript(FEScript *, bool);
    unsigned char unknown_14[0x18 - 0x14];
    int type;                     // +0x18; 5 for a group, 2 for a string in SetGroupString.
    unsigned int unknownBits : 5; // +0x1c, the object's flag word.
    unsigned int disabled : 1;    // Set by DisableButton.
    unsigned int unknownBit6 : 3;
    unsigned int changed : 1;     // Set whenever text or script is replaced.
    unsigned char unknown_20[0x2c - 0x20];
    unsigned char *data;          // +0x2c
    unsigned long dataSize;       // +0x30
    unsigned char unknown_34[0x38 - 0x34];
    FEMessageResponse *responses; // +0x38
    unsigned char unknown_3c[0x48 - 0x3c];
    FEScript *scripts;            // +0x48
    unsigned char unknown_4c[0x54 - 0x4c];
    FEScript *currentScript;      // +0x54
};

class FEGroup : public FEObject {
  public:
    FEObject *FindChild(unsigned long) const;
    FEObject *FindChild(const char *) const;
    unsigned char unknown_58[0x5c - 0x58];
    FEObject *children; // +0x5c
};

// A growable array of the objects that receive one message.
class FEMsgTargetList {
  public:
    unsigned long message;   // +0x0
    unsigned long allocated; // +0x4
    unsigned long count;     // +0x8
    FEObject **targets;      // +0xc
    void Allocate(unsigned long);
    void AppendTarget(FEObject *);
};

class FEPackage : public FENode {
  public:
    FEMsgTargetList *GetMessageTargets(unsigned long);
    FEObject *FindObjectByName(const char *);
    FEObject *FindObjectByHash(unsigned long);
    int executing;                    // +0x14
    unsigned char unknown_18[0x24 - 0x18];
    int priority;                     // +0x24
    int controlMask;                  // +0x28
    unsigned char unknown_2c[0x38 - 0x2c];
    const char *fileName;             // +0x38
    const FEPackage *parent;          // +0x3c
    unsigned char unknown_40[0x48 - 0x40];
    FEObject *firstObject;            // +0x48
    unsigned char unknown_4c[0x6c - 0x4c];
    unsigned long messageTargetCount; // +0x6c
    FEMsgTargetList *messageTargets;  // +0x70
    unsigned char unknown_74[0xac - 0x74];
    signed char groupId;              // +0xac
};

// Packages sorted by priority.
class FEPackageList : public FEMinList {
  public:
    void AddPackage(FEPackage *);
    FEPackage *FindPackage(const char *, unsigned char) const;
    FEPackage *FindPackage(unsigned char) const;
    int RemovePackage(FEPackage *);
    void ReplaceParentLinks(const FEPackage *, const FEPackage *);
};

unsigned long FEHashUpper(const char *);
int FEStricmp(const char *, const char *);

#endif
