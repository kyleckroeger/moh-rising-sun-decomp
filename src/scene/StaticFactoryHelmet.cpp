// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original staticobjfactory.cpp. CAnyStaticMesh's symbol names it;
// its layout, the definition record and the member names are inferred from their accesses.
#include "SceneObjects.h"
#include <stdlib.h>

void DebugMsg(const char *, ...);
class PickupInfo;
class IGameObject {
  public:
    void IGO_Destroy();
};
class CGameObjectFactory {
  public:
    void Reset();
};
class CStaticMesh {
  public:
    unsigned char unknown00[0x18];
    virtual ~CStaticMesh(); // Table pointer at +0x18.
};
// A mesh slot whose type word is negative when empty.
class CAnyStaticMesh {
  public:
    int type;
    unsigned char storage[40];
    CStaticMesh *ptr();
    CAnyStaticMesh() { type = -1; }
    ~CAnyStaticMesh() {
        if (ptr())
            ptr()->~CStaticMesh();
        type = -1;
    }
};
// A 112-byte sorted template record; only the fields below are established.
class StaticObjectDefinition {
  public:
    CAnyStaticMesh mesh;
    int id;           // +0x2c
    unsigned char unknown30[4];
    MotionData *motion; // +0x34
    unsigned char unknown38[8];
    int variant;      // +0x40
    unsigned char unknown44[112 - 0x44];
};
void DebugMsg(const char *, ...);
class CStaticObjectFactory {
  public:
    static bool DoesHelmetTypeStopBullets(int);
};
extern StaticObjectDefinition *g_pStaticObjectTemplates;
extern int g_numTemplates;
int CompareStaticObjectDefinitions(const void *, const void *);

bool CStaticObjectFactory::DoesHelmetTypeStopBullets(int type) {
    StaticObjectDefinition key;
    key.id = type;
    key.variant = -1;
    StaticObjectDefinition *found = (StaticObjectDefinition *)bsearch(&key, g_pStaticObjectTemplates, g_numTemplates,
                                                                      sizeof(StaticObjectDefinition), CompareStaticObjectDefinitions);
    if (!found)
        DebugMsg("ERROR: Couldn't find helmet type %d\n", type);
    // The definition data at +60 leads through +8 and +80 to the word that decides this; names unknown.
    return !(*(int **)((*(char ***)((char *)found + 60))[2] + 80))[2];
}
