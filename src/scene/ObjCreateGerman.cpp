// AI-assisted reconstruction from GR8E69. Part of the original objcreate.cpp; local names are
// descriptive. The address locals and ZOf are inferred from the saved registers and the gcse copy of
// the position's address, and the unused last parameter's type comes from the mangled name.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "ScriptTimers.h"
#include "AIDoodad.h"
#include "ScriptStartup.h"
#include "ObjCreate.h"

void DebugMsg(const char *, ...);
void ForceUpdateProximityTriggerStatus(BSGO_Basic *, bool);
void QueueCreateGerman(TriggerObject *, int, int);
enum EObjectClass { EObjectClassUnknown0 = 0, EObjectClassUnknown1 = 1 };
class CAnimObjectFactory {
  public:
    CAnimObject *AllocateObject(unsigned int, EObjectClass);
};
extern CAnimObjectFactory g_AnimObjFactory;

static inline float ZOf(const CVector3 &v) { return v.z; }

BSGO_Soldier *CreateGerman(TriggerObject *trigger, int value, int animation, bool queue, bool *) {
    if (g_numActiveSoldiers >= g_maxActiveSoldiers) {
        if (queue)
            QueueCreateGerman(trigger, value, animation);
        return 0;
    }
    g_numActiveSoldiers++;
    BSGO_Soldier *go = new BSGO_Soldier;
    AddObjectToList(go, (BSGO_Basic **)&g_pGermanList);
    CAnimObject *anim = g_AnimObjFactory.AllocateObject(animation, EObjectClassUnknown1);
    CSoldierObject *soldier = (CSoldierObject *)anim;
    CAnimObjectInterface *self = (CAnimObjectInterface *)anim;
    int collision;
    switch (trigger->GetInt("Team")) {
    case 1:
        collision = 5;
        break;
    case 2:
        collision = 2;
        g_iNumberOfEnemiesSpawned++;
        break;
    case 0:
        collision = 7;
        break;
    default:
        collision = -1;
        DebugMsg("Unknown team affiliation\n");
    }
    self->SetCollisionId((EClsnId)collision);
    if (trigger->GetCRC("WeaponSlot1") == 0)
        soldier->AddWeapon(0, 0x5760e46d, 999);
    else
        soldier->AddWeapon(0, trigger->GetCRC("WeaponSlot1"), 999);
    soldier->AddWeapon(1, trigger->GetCRC("WeaponSlot2"), 999);
    soldier->AddWeapon(2, trigger->GetCRC("WeaponSlot3"), 999);
    soldier->AddWeapon(2, trigger->GetCRC("GrenadeSlot"), 999);
    if (trigger->GetCRC("Prop"))
        self->AttachObject(trigger->GetCRC("Prop"), "JO_prop", 0);
    CVector3 position;
    CMatrix matrix;
    float angleX, angleY, angleZ;
    float *addressX = &angleX;
    trigger->GetPosition(position);
    int zero = 0;
    float *addressZ;
    float *addressY;
    CMatrix *matrixAddress;
    addressY = &angleY;
    addressZ = &angleZ;
    matrixAddress = &matrix;
    self->Reset();
    self->SetPosition(position);
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    trigger->GetMatrix(*matrixAddress);
    matrixAddress->ToEulerXYZ(*addressX, *addressY, *addressZ);
    self->Yaw(angleZ);
    g_scene.Add(*(ISceneNode *)anim, 0);
    go->target.Assign((ISubject *)anim);
    go->proximityData[1] = zero;
    go->proximityData[0] = zero;
    go->proximityData[3] = zero;
    go->proximityData[2] = zero;
    go->scriptData.hitPoints = trigger->GetInt("HitPoints");
    go->scriptData.value40 = zero;
    go->scriptData.value44 = zero;
    go->scriptData.value48 = zero;
    go->scriptData.value4c = zero;
    go->scriptData.value50 = 2;
    go->scriptData.value3c = 1;
    if (trigger->GetChildScript()) {
        CreateBSObject(trigger->GetChildScript(), &go->script, go, trigger, &go->scriptData.scriptState);
        self->SetScript(go->script);
        BSObjectTriggerEvent(go->script, 41, (void *)value, 0, false);
    } else {
        DebugMsg("SOON TO BE ERROR! Soldier at x: %d  y: %d  z: %d  has no child script.  Thats bad\n", (int)position.x, (int)position.y, (int)ZOf(position));
#define REPEAT DebugMsg("(repeated for importance)! Soldier at x: %d  y: %d  z: %d  has no child script.  Thats bad\n", (int)position.x, (int)position.y, (int)ZOf(position));
        REPEAT REPEAT REPEAT REPEAT REPEAT REPEAT REPEAT REPEAT REPEAT
    }
    CAISoldierDoodad *doodad = (CAISoldierDoodad *)soldier->aiDoodad;
    soldier->InitAI();
    soldier->SetCurrentWeapon(0, 0, false);
    position.Set(0.0f, 0.0f, 1.0f);
    doodad->soldier->SetUpperTranslation(position);
    doodad->soldier->SetUpperXAngleDifference(-1.2217306f, 1.5707965f);
    doodad->soldier->SetUpperZAngleDifference(-0.3490659f, 0.3490659f);
    ForceUpdateProximityTriggerStatus(go, false);
    return go;
}

void *CreateBasicAnimatedMech(TriggerObject *trigger, int value, int animation) {
    int zero = 0;
    BSGO_Anim *go = new BSGO_Anim;
    go->proximityData[1] = zero;
    go->proximityData[0] = zero;
    go->proximityData[3] = zero;
    go->proximityData[2] = zero;
    go->script = 0;
    go->target.Assign(0);
    AddObjectToList(go, (BSGO_Basic **)&g_pAnimatedMechList);
    CAnimObject *anim = g_AnimObjFactory.AllocateObject(animation, EObjectClassUnknown0);
    CAnimObjectInterface *self = (CAnimObjectInterface *)anim;
    CVector3 position;
    CMatrix matrix;
    float angleX, angleY, angleZ;
    float *addressX = &angleX;
    float *addressZ;
    float *addressY;
    CMatrix *matrixAddress;
    addressY = &angleY;
    addressZ = &angleZ;
    matrixAddress = &matrix;
    trigger->GetPosition(position);
    self->Reset();
    self->SetPosition(position);
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    trigger->GetMatrix(*matrixAddress);
    matrixAddress->ToEulerXYZ(*addressX, *addressY, *addressZ);
    self->Yaw(angleZ);
    g_scene.Add(*(ISceneNode *)anim, 0);
    go->target.Assign((ISubject *)anim);
    if (trigger->GetChildScript()) {
        CreateBSObject(trigger->GetChildScript(), &go->script, go, trigger, &go->scriptData);
        self->SetScript(go->script);
        BSObjectTriggerEvent(go->script, 41, (void *)value, 0, false);
    } else {
        DebugMsg("SOON TO BE ERROR! Animated Mech at x: %d y: %d z: %d has no child script. Thats bad\n", (int)position.x, (int)position.y, (int)ZOf(position));
        for (int i = 0; i < 9; i++)
            DebugMsg("(repeated for importance)! Animated Mech at x: %d y: %d z: %d has no child script. Thats bad\n", (int)position.x, (int)position.y, (int)ZOf(position));
    }
    return go;
}
