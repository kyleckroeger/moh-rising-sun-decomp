// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

void CHierObject::CollisionResetMovement() {
    scriptMoveStep = 0.0f;
    scriptRotateStep = 0.0f;
    scriptTargetOrientation = orientation;
    attachDelta.Zero();
    if (pathState > 1) {
        transform.SetPos(pathPosition);
        pathAngle = 0.0f;
        pathDistance = 0.0f;
    } else if (ScriptMoving() || ScriptRotating()) {
        CMatrix *matrix = &transform;
        orientation.GetMatrix(*matrix);
        CMatrix *attach = &attachPointMatrix;
        matrix->Translate(attachOffset);
        CMatrix *inherited = &inheritedMatrix;
        CMatrix::s_TempMat = *matrix;
        matrix->Multiply(CMatrix::s_TempMat, *attach);
        CMatrix::s_TempMat = *matrix;
        matrix->Multiply(CMatrix::s_TempMat, *inherited);
    } else if (flags.motionPlaying) {
        velocity.Zero();
        motionResetOrientation = animOrientation;
    }
}
