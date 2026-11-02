// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

// The light reference at +0x294: a weak reference whose subject sits at +0x14.
struct AttachedLightRef {
    unsigned char unknown_0[0x14];
    CLight *subject;
    operator bool() const { return subject != 0; }
};

void CStaticObject::CommitUpdate() {
    IVolume *world = worldVolumeType3;
    if (world != (IVolume *)field_13c)
        world->TransformedCopy(*worldVolume, transform);
    if (pathState > 1) {
        ((CStaticObjectInterface *)this)->CommitPathMotionUpdate();
    } else if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
        ((CStaticObjectInterface *)this)->CommitScriptMotionUpdate();
    } else if (flags.motionPlaying) {
        ((CStaticObjectInterface *)this)->CommitAnimMotionUpdate();
    }
    AttachedLightRef &light = *(AttachedLightRef *)unknown_294;
    if (light) {
        CMatrix matrix;
        CMatrix *saved = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        ((ISceneNode *)this)->GetTMLocalToWorld(*saved);
        saved->PreTranslate(attachedLightOffset);
        ((IMovingSceneNode *)light.subject)->SetTMLocalToWorld(*saved);
    }
}
