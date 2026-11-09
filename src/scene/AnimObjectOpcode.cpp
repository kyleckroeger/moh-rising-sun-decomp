// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive. The opcode handler
// at the end serves the choreographer: script events, sounds, head tracking, sword state and
// controller feedback.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "EAGLTransform.h"
#include "CameraShake.h"
#include "Shell.h"
extern "C" int sprintf(char *, const char *, ...);
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
void PlayEnemyMovement(unsigned short, int, CAnimObject *);
void PlayAnimTriggeredSound(int, CAnimObject *);
void PlayReloadSound(int, CAnimObject *);

class CAnimObjectFactory {
  public:
    void ReleaseObject(CAnimObject *);
};
extern CAnimObjectFactory g_AnimObjFactory;

void CAnimObject::SetAttached(bool value) {
    attached = value;
    attachedToLocation = 0;
    attachBlend = -1.0f;
}

COORD3 CAnimObject::GetPosition() {
    COORD3 position;
    ((ISceneNode *)this)->GetPosition(*(CVector3 *)&position);
    return position;
}

float CAnimObject::GetRotation() { return GetYaw(); }

void CAnimObject::SetPosition(const COORD3 &position) {
    ((IMovingSceneNode *)this)->SetPosition(*(const CVector3 *)&position);
}

void CAnimObject::SetRotation(const float &angle) { SetYaw(angle); }

void CAnimObject::MarkForDestruction(int flag) {
    ((ISubject *)this)->ISubject::MarkForDestruction(flag);
    g_scene.Remove(*(ISceneNode *)this);
}

void CAnimObject::Destroy() { g_AnimObjFactory.ReleaseObject(this); }

int CAnimObject::AO_OpcodeHandler::Execute(EALA::Character::Opcode &opcode) {
    int result = 1;
    int param = (opcode.paramHigh << 16) + opcode.paramLow;
    unsigned short duration = opcode.extra;
    unsigned short id = opcode.id;
    switch (id) {
    case 0xc804:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(126, 0, false);
        break;
    case 0xc805:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(128, 0, false);
        break;
    case 0xc807:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(130, 0, false);
        break;
    case 0xc808:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(219, 0, false);
        break;
    case 0xc809:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(220, 0, false);
        break;
    case 0xc80a:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(169, 0, false);
        break;
    case 0xc810:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(238, 0, false);
        break;
    case 0xc80c:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(180, 0, false);
        break;
    case 0xca1e:
        if (param == 3)
            ((CAnimObjectInterface *)owner)->TriggerScriptEvent(250, 0, false);
        PlayEnemyMovement(0xca1e, param, owner);
        break;
    case 0xca00: case 0xca0a: case 0xca13: case 0xca16: case 0xca18: case 0xca19:
    case 0xca22: case 0xca24: case 0xca29: case 0xca2f: case 0xca34: case 0xca38:
    case 0xca39: case 0xca3b: case 0xca3d: case 0xca43: case 0xca53: case 0xca5a:
    case 0xca5d:
        PlayEnemyMovement(id, param, owner);
        break;
    case 0xcaf0:
        PlayAnimTriggeredSound(param, owner);
        break;
    case 0xc806:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(129, (void *)param, false);
        PlayReloadSound(param, owner);
        break;
    case 0xc803:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(127, (void *)param, false);
        break;
    case 0xc80d:
        ((CAnimObjectInterface *)owner)->AsSoldierObject()->SetHeadTrackPlayer(true);
        break;
    case 0xc80e:
        ((CAnimObjectInterface *)owner)->AsSoldierObject()->SetHeadTrackPlayer(false);
        break;
    case 0xc811:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(240, (void *)(unsigned short)param, false);
        break;
    case 0xc80b: {
        unsigned short motor = param;
        int port = 0;
        CPlayerWeaponObject *weapon = ((CAnimObjectInterface *)owner)->AsPlayerWeaponObject();
        // The binary tests these flags, but both paths do the same thing; the form is inferred.
        if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
            if (weapon)
                port = ((CAnimObjectInterface *)weapon->player)->GetPlayerIndex();
        } else {
            if (weapon)
                port = ((CAnimObjectInterface *)weapon->player)->GetPlayerIndex();
        }
        if (!duration)
            duration = 1;
        g_inputMgr.SetFeedback(port, motor, 0, 1.0f, (float)(unsigned int)duration * 0.1f);
        break;
    }
    case 0xca80:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(param + 356, (void *)param, false);
        break;
    case 0xca81:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(138, (void *)(param == 0), false);
        break;
    case 0xca82:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(131, 0, false);
        break;
    case 0xcb00:
        owner->stateDone = 1;
        break;
    case 0xcb01:
        owner->stateDone = 0;
        break;
    case 0xcb02:
        ((CAnimObjectInterface *)owner)->OnSwordDraw();
        break;
    case 0xcb03:
        ((CAnimObjectInterface *)owner)->OnSwordSheath();
        break;
    case 0xcb04:
        ((CAnimObjectInterface *)owner)->TriggerScriptEvent(472, 0, false);
        break;
    case 0xc80f:
    case 0xc900:
        break;
    default: {
        char message[80];
        result = 0;
        sprintf(message, "Unknown or Unhandled opcode: %04x\n", id);
        break;
    }
    }
    return result;
}
