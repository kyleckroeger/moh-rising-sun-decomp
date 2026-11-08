// AI-assisted reconstruction from GR8E69. Part of the original objcreate.cpp. The function name and
// its messages are original; local names are descriptive.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "ScriptTimers.h"
#include "ObjCreate.h"

void DebugMsg(const char *, ...);

// Asks the soldiers farthest from the player to remove themselves when there is not room for
// `wanted` more. The event number is the original constant; its name is not known.
void NonVisDestructFarthestSoldiers(int wanted) {
    int excess = wanted - (g_maxActiveSoldiers - g_numActiveSoldiers);
    if (excess > 0) {
        int count = 0;
        float best = 0.0f; // Never read: the loop resets it. Inferred from the literal order and register use.
        DebugMsg("-----The maximum number of soldiers in existance has been reached.\n");
        DebugMsg("-----This number is currently %d.  It is controled by commondata.crp\n", g_maxActiveSoldiers);
        DebugMsg("-----Attempting to remove %d soldiers\n", excess);
        float distances[128];
        CVector3 playerPosition;
        CVector3 position;
        CSoldierObject *soldiers[128];
        ((ISceneNode *)g_scene.GetPlayer(0))->GetPosition(playerPosition);
        for (BSGO_Basic *object = (BSGO_Basic *)g_pGermanList; object; object = object->next, count++) {
            CSoldierObject *soldier = (CSoldierObject *)object->GetSceneNode();
            if (soldier->nonVisDestructDisabled) {
                count--;
                continue;
            }
            ((ISceneNode *)soldier)->GetPosition(position);
            position -= playerPosition;
            soldiers[count] = soldier;
            float distance = position.x * position.x + position.y * position.y;
            distance += position.z * position.z;
            distances[count] = distance;
        }
        float limit = 1e35f;
        int removed = 0;
        while (removed < excess) {
            CSoldierObject *farthest = 0;
            best = 0.0f;
            for (int i = 0; i < count; i++) {
                if (distances[i] <= best || distances[i] >= limit)
                    continue;
                farthest = soldiers[i];
                best = distances[i];
            }
            limit = best;
            if (farthest) {
                if (BSObjectTriggerEvent((BSObject *)((ISceneNode *)farthest)->GetScriptObject(), 42, 0, 0, true)) {
                    removed++;
                    DebugMsg("-----I found a suitable candidate.\n");
                }
            } else {
                DebugMsg("WARNING:WARNING: I tried to NonVisDestruct %d soldiers because there\n", excess);
                DebugMsg("WARNING:WARNING: are too many in existance, but I was only able to\n");
                DebugMsg("WARNING:WARNING: NonVisDestruct %d soldiers.  I'll try again later\n", removed);
                break;
            }
        }
    }
}
