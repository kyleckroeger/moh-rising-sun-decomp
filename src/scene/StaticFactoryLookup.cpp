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
class CStaticObjectFactory {
  public:
    static CGameObjectFactory m_StaticObjectFactory;
    static CGameObjectFactory m_ThrownObjectFactory;
    static CGameObjectFactory m_RecycledThrownObjectFactory;
    static CGameObjectFactory m_AttachObjectFactory;
    static CGameObjectFactory m_HierObjectFactory;
    static CGameObjectFactory m_TankObjectFactory;
    static CGameObjectFactory m_HierAnimObjectFactory;
    static void DeallocateStaticObject(CStaticObject *);
    static CStaticMesh *GetStaticMesh(int, int);
    static MotionData *GetMotionData(int, int);
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
    static void Reset();
};
extern StaticObjectDefinition *g_pStaticObjectTemplates;
extern int g_numTemplates;
int CompareStaticObjectDefinitions(const void *, const void *);
extern int g_iMsgVerbosityLevel;
extern CStaticObject *g_pStaticObjectGrenade, *g_pStaticObjectBazooka, *g_pStaticObjectStickGrenade,
    *g_pStaticObjectKnife, *g_pStaticObjectWrench;

void CStaticObjectFactory::DeallocateStaticObject(CStaticObject *object) {
    if (object) {
        if (g_iMsgVerbosityLevel > 0)
            DebugMsg("Deallocating static object %p <<<d %p, t %d>>>\n", object, object,
                     ((CStaticObjectInterface *)object)->GetObjectType());
        ((IGameObject *)((char *)object + 0x38))->IGO_Destroy();
    }
}

CStaticMesh *CStaticObjectFactory::GetStaticMesh(int id, int variant) {
    StaticObjectDefinition key;
    key.id = id;
    key.variant = variant;
    StaticObjectDefinition *found = (StaticObjectDefinition *)bsearch(&key, g_pStaticObjectTemplates, g_numTemplates,
                                                                      sizeof(StaticObjectDefinition), CompareStaticObjectDefinitions);
    if (!found)
        return 0;
    return found->mesh.type >= 0 ? (CStaticMesh *)found->mesh.storage : 0;
}

MotionData *CStaticObjectFactory::GetMotionData(int id, int variant) {
    StaticObjectDefinition key;
    key.id = id;
    key.variant = variant;
    StaticObjectDefinition *found = (StaticObjectDefinition *)bsearch(&key, g_pStaticObjectTemplates, g_numTemplates,
                                                                      sizeof(StaticObjectDefinition), CompareStaticObjectDefinitions);
    if (!found)
        return 0;
    return found->motion;
}

void CStaticObjectFactory::Reset() {
    m_StaticObjectFactory.Reset();
    m_ThrownObjectFactory.Reset();
    m_RecycledThrownObjectFactory.Reset();
    m_AttachObjectFactory.Reset();
    m_HierObjectFactory.Reset();
    m_HierAnimObjectFactory.Reset();
    m_TankObjectFactory.Reset();
    g_pStaticObjectGrenade = AllocateStaticObject(0xa55dda2d, -1, 0);
    g_pStaticObjectStickGrenade = AllocateStaticObject(0x83674487, -1, 0);
    g_pStaticObjectKnife = AllocateStaticObject(0x7b81b72c, -1, 0);
    g_pStaticObjectWrench = AllocateStaticObject(0x60531a56, -1, 0);
    g_pStaticObjectBazooka = AllocateStaticObject(0x55c5e1cf, -1, 0);
}
