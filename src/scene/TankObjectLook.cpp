// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include "SceneObjects.h"

void CTankObject::GetLookMatrix(CMatrix &out) {
    if (lookObject) {
        out = GetSubObject(lookObject)->transform;
    } else {
        CVector3 position;
        CVector3 *saved = &position;
        out = transform;
        position = out.GetPos();
        saved->z += 1.0f;
        out.SetPos(position);
    }
}
