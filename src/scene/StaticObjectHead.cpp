// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CStaticObject::InitMotionProperties(CStaticMesh *mesh, int frameCount, int speed, int loop, int rate, int enabled, MotionFrame *frames) {
    model = mesh;
    motionFrameCount = frameCount;
    motionSpeed = speed;
    motionLoop = loop;
    motionRate = rate;
    flags.hasMotion = enabled != 0;
    motionKey = 0;
    motionFrames = frames;
    motionTimer = 0;
}


void CStaticObject::SetLocalBoundingVolume(ISceneNode::EVolumeType type, IVolume *volume) {
    switch (type) {
    case 3:
        localVolumeType3 = volume;
        break;
    case 0:
        worldVolume = volume;
        break;
    case 1:
    case 2:
        break;
    }
}


void CStaticObject::SetWorldBoundingVolume(ISceneNode::EVolumeType type, IVolume *volume) {
    switch (type) {
    case 3:
        if (volume != (IVolume *)field_13c)
            volume->TransformedCopy(*volume, transform);
        worldVolumeType3 = volume;
        break;
    case 0:
        if (volume != worldVolumeType3)
            volume->TransformedCopy(*volume, transform);
        field_13c = (int)volume;
        if (((ISceneNode *)this)->GetCollisionId() == (EClsnId)-1)
            ((ISceneNode *)this)->SetCollisionId((EClsnId)0);
        break;
    case 1:
    case 2:
        break;
    }
}


IVolume *CStaticObject::GetLocalBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return localVolumeType3;
    case 0:
    case 1:
    case 2:
        return worldVolume;
    case 4:
        return 0;
    }
    return 0;
}


IVolume *CStaticObject::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return worldVolumeType3;
    case 0:
    case 1:
    case 2:
        return (IVolume *)field_13c;
    case 4:
        return 0;
    }
    return 0;
}

