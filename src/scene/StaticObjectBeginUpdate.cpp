// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "Volume.h"

extern int g_bInMultiplayerMode;

// The static mesh carries a table pointer at +0x18 and is updated through its +0x10 slot;
// the leading slot and this view's name are placeholders for that position.
class CStaticMeshUpdateView {
  public:
    unsigned char unknown_0[0x18];
    virtual void Slot8();
    virtual void Update(float);
};

void CStaticObject::BeginUpdate(float dt) {
    float timer = destructionTimer;
    if (timer != 3.4028235e38f) {
        if (!(timer <= 0.0f)) {
            float next = timer - dt;
            destructionTimer = next;
            if (!(next > 0.0f))
                destructionTimer = 0.0f;
        } else if (!(timer < 0.0f)) {
            ((ISubject *)this)->MarkForDestruction(1);
            return;
        } else {
            float next = timer + 1.0f;
            destructionTimer = next;
            if (!(next < 0.0f))
                destructionTimer = 0.0f;
        }
    }
    lightVolumes.Update(dt);
    fade.Update(dt * 0.0166666675f);
    AnimUpdate(dt);
    if (pathState > 1) {
        BeginPathMotionUpdate(dt);
    } else {
        Flags current = flags;
        if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
            BeginScriptMotionUpdate(dt);
            flags.attached = 0;
        } else if (current.motionPlaying) {
            BeginAnimMotionUpdate(dt);
        } else {
            if (g_bInMultiplayerMode && current.dumpObject)
                dumpAge += dt * 0.012f;
            IVolume *world = (IVolume *)field_13c;
            if (world) {
                CVector3 min, max;
                world->GetExtents(min, max);
                sortMinX->value = min.x;
                sortMinY->value = min.y;
                sortMaxX->value = max.x;
                sortMaxY->value = max.y;
            } else {
                CVector3 position;
                ((ISceneNode *)this)->GetPosition(position);
                sortMinX->value = position.x;
                sortMinY->value = position.y;
                sortMaxX->value = position.x;
                sortMaxY->value = position.y;
            }
        }
    }
    ((CStaticMeshUpdateView *)model)->Update(dt);
}
