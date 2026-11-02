// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "CSGVolume.h"
#include "CameraShake.h"

void CHierObject::AttemptUpdateFromMotion(float dt) {
    if (pathState > 1) {
        ((CStaticObjectInterface *)this)->AttemptPathMotionUpdate(dt);
    } else if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
        ((CStaticObjectInterface *)this)->AttemptScriptMotionUpdate(dt);
    } else if (flags.motionPlaying) {
        ((CStaticObjectInterface *)this)->AttemptAnimMotionUpdate(dt);
    } else {
        CMatrix *matrix = &transform;
        orientation.GetMatrix(*matrix);
        CMatrix *attach = &attachPointMatrix;
        matrix->SetPos(attachOffset);
        CMatrix *inherited = &inheritedMatrix;
        CMatrix::s_TempMat = *matrix;
        matrix->Multiply(CMatrix::s_TempMat, *attach);
        CMatrix::s_TempMat = *matrix;
        matrix->Multiply(CMatrix::s_TempMat, *inherited);
    }
}

