// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AIThreats.cpp. The stack layout of MakeNoise is read from the
// image: the source object compared in the loop is never written in this build.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "ScriptRuntime.h"
#include "BSGameObject.h"
#include "AISplinePath.h"
#include "AITargeting.h"

struct NoiseScriptData {
    unsigned char unknown00[0x10];
    int value10; // +0x10
    int value14; // +0x14
};
// The sound description on the stack. Only the default-constructed position and the ignored object
// are observed; the ignored object is compared without being written in this build.
struct NoiseInfo {
    void *source;       // +0x00
    CAIObject *ignored; // +0x04
    int type;           // +0x08
    int unknown0c;
    CVector3 position;  // +0x10
    unsigned char unknown20[0x20];
};
void MakeNoise(void *, CAIObject *filter, const CVector3 &position, int type, float loudness, bool, int) {
    NoiseInfo info;
    if (type == 48) {
        BSObject *script = (BSObject *)((ISceneNode *)g_scene.GetPlayer(0))->GetScriptObject();
        NoiseScriptData *data = (NoiseScriptData *)script->gameObject->GetScriptData();
        if (data->value10 && data->value14)
            return;
    }
    if (!(loudness >= 0.0f))
        loudness = 0.0f;
    if (!(loudness <= 6.0f))
        loudness = 6.0f;
    bool onNetwork = AIPathFinding::IsPointOnNetwork(position);
    for (CAIObject *other = CAIObject::GetGlobalListStart(); other; other = other->nextGlobal) {
        if (info.ignored != other && ((CAIObjectInterface *)other)->AsAISoldierObject()) {
            CAISoldierObject *soldier = ((CAIObjectInterface *)other)->AsAISoldierObject();
            if (soldier)
                soldier->threatEvaluation.PropagateSoundToMe(type, loudness, position, filter, onNetwork);
        }
    }
}

void MakeNoise(ISceneNode *node, int type, float loudness, int value) {
    CAIObject *filter = 0;
    if (node)
        filter = node->GetAIObject();
    CVector3 position;
    if (node)
        node->GetPosition(position);
    MakeNoise(node, filter, position, type, loudness, true, value);
}
