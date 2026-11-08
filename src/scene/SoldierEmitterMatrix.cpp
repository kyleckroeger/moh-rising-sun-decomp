// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

void CSoldierObject::GetBulletEmitterMatrix(CMatrix &out) {
    if (emitterActive && attachSlots[emitterSlot].active) {
        out = ((CAnimObject *)this)->transform;
        const CMatrix *emitter = &emitterMatrix;
        const CMatrix *point = attachSlots[emitterSlot].matrix;
        CMatrix::s_TempMat = out;
        out.Multiply(*point, CMatrix::s_TempMat);
        CMatrix::s_TempMat = out;
        out.Multiply(*emitter, CMatrix::s_TempMat);
    } else {
        out = defaultEmitterMatrix;
    }
}
