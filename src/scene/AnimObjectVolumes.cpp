// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "EAGLTransform.h"
#include "ScriptRuntime.h"

extern int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
class CAnimObjectFactory {
  public:
    void ReleaseObject(CAnimObject *);
};
extern CAnimObjectFactory g_AnimObjFactory;

void CAnimObject::SetLocalBoundingVolume(ISceneNode::EVolumeType type, IVolume *volume) {
    switch (type) {
    case 3:
        localVolumeType3 = volume;
        break;
    case 0:
        localVolumeType0 = volume;
        break;
    case 1:
    case 2:
        break;
    }
}

void CAnimObject::SetWorldBoundingVolume(ISceneNode::EVolumeType type, IVolume *volume) {
    switch (type) {
    case 3:
        worldVolumeType3 = volume;
        break;
    case 0:
        worldVolumeType0 = volume;
        break;
    case 1:
    case 2:
        break;
    }
}

IVolume *CAnimObject::GetLocalBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return localVolumeType3;
    case 0:
    case 1:
    case 2:
        return localVolumeType0;
    case 4:
        return 0;
    }
    return 0;
}

IVolume *CAnimObject::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return worldVolumeType3;
    case 0:
    case 1:
    case 2:
        return worldVolumeType0;
    case 4:
        return 0;
    }
    return 0;
}

void CAnimObject::TriggerScriptEvent(int event, void *data, bool immediate) {
    if (script)
        BSObjectTriggerEvent(script, event, data, 0, immediate);
}
