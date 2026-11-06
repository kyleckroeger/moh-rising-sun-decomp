// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "GameSettings.h"
#include "ObserverTypes.h"
#include "ScriptRuntime.h"
#include "BSGameObject.h"
#include "TriggerObject.h"
#include "Volume.h"

void DestroyDumpObject(CStaticObject *);
class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};

void CStaticObject::MarkForDestruction(int flag) {
    ((ISubject *)this)->ISubject::MarkForDestruction(flag);
    g_scene.Remove(*(ISceneNode *)this);
    if (script)
        script->queueIdentity = ~0U;
}

void CStaticObject::Destroy() {
    if (script && script->gameObject)
        script->gameObject->Destroy();
    else if (flags.dumpObject)
        DestroyDumpObject(this);
    else
        CStaticObjectFactory::DeallocateStaticObject(this);
}

extern int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void CStaticObject::TriggerScriptEvent(unsigned short event, void *data, bool immediate) {
    BSObjectTriggerEvent(script, event, data, 0, immediate);
}

void CStaticObject::InitAim(bool enabled, float value) {
    aimValue = value;
    flags.aimEnabled = enabled;
}

void CStaticObject::FadeOut(float duration) { fade.StartFadeOut(duration, 0.0f); }
void CStaticObject::FadeIn(float duration) { fade.StartFadeIn(duration, 0.0f); }
float CStaticObject::FadeValue() const { return fade.FadeValue(); }

void CStaticObject::SerializeSaveData() {
    CMatrix matrix;
    CMatrix *saved = &matrix;
    *saved = transform;
    if (g_GameSettings.SerializingOut() && script && script->nativeObject)
        script->nativeObject->GetMatrix(*saved);
    g_GameSettings.SerializeDelta(matrix, transform);
    if (g_GameSettings.SerializingIn()) {
        orientation.SetFromMatrix(transform);
        IVolume *world = (IVolume *)field_13c;
        if (world) {
            world->TransformedCopy(*worldVolume, transform);
            CVector3 min, max;
            ((IVolume *)field_13c)->GetExtents(min, max);
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
