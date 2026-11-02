// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

// The static mesh carries a table pointer at +0x18 and is updated through its +0x10 slot;
// the leading slot and this view's name are placeholders for that position.
class CStaticMeshUpdateView {
  public:
    unsigned char unknown_0[0x18];
    virtual void Slot8();
    virtual void Update(float);
};

void CHierObject::DoBeginUpdate(float dt, bool checkMounted) {
    if (checkMounted && hierFlags.mountedGun)
        return;
    AnimUpdate(dt);
    if (pathState > 1) {
        BeginPathMotionUpdate(dt);
    } else if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
        ((CStaticObjectInterface *)this)->BeginScriptMotionUpdate(dt);
    } else if (flags.motionPlaying) {
        ((CStaticObjectInterface *)this)->BeginAnimMotionUpdate(dt);
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
        IVolume *world = (IVolume *)field_13c;
        if (world && worldVolume)
            world->TransformedCopy(*worldVolume, *matrix);
    }
    for (CHierObject *child = firstChild; child; child = child->nextSibling) {
        child->SetInheritedMatrix(transform);
        child->DoBeginUpdate(dt, 1);
        CStaticMeshUpdateView *mesh = (CStaticMeshUpdateView *)child->model;
        if (mesh)
            mesh->Update(dt);
    }
}
