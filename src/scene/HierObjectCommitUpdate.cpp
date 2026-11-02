// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CHierObject::CommitUpdate() {
    IVolume *world = worldVolumeType3;
    if (world != (IVolume *)field_13c)
        world->TransformedCopy(*worldVolume, transform);
    if (pathState > 1) {
        ((CStaticObjectInterface *)this)->CommitPathMotionUpdate();
        attachOffset = transform.GetPos();
        orientation.SetFromMatrix(transform);
    } else if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
        ((CStaticObjectInterface *)this)->CommitScriptMotionUpdate();
    } else if (flags.motionPlaying) {
        ((CStaticObjectInterface *)this)->CommitAnimMotionUpdate();
    }
    for (CHierObject *child = firstChild; child; child = child->nextSibling)
        ((ISceneNode *)child)->CommitUpdate();
    hierFlags.collided = 0;
}
