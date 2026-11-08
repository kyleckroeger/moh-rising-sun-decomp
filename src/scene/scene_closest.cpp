// CScene::FindClosestPlayer returns the player nearest a point.
#include "SceneNode.h"
#include "CVector3.h"
#include <math.h>

CPlayerObject *CScene::FindClosestPlayer(const CVector3 &point) {
    float best = 3.4028235e+38f;
    CPlayerObject *closest = 0;
    CVector3 position;
    for (int i = 0; i < playerCount; i++) {
        ((ISceneNode *)players[i])->GetPosition(position);
        position -= point;
        float distance = sqrtf(position.LengthSquared());
        if (!(distance >= best)) {
            closest = players[i];
            best = distance;
        }
    }
    return closest;
}
