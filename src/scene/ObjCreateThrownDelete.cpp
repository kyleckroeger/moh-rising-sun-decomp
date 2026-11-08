// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
#include "SceneObjects.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "BSGameObject.h"
#include "ObjCreate.h"

class CThrownObject;
// A thrown object's table has no FindAttachNode, so CleanUp sits one slot earlier than in the
// attachable table CStaticObjectInterface describes.
class CThrownObjectInterface : public IMovingSceneNode {
  public:
    virtual void Init();
    virtual void SetScript(BSObject *);
    virtual int GetObjectType() const;
    virtual void TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    virtual void RotateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    virtual void ScaleFromScript(float, float, EBSEventEnum);
    virtual void StartMotionPlayback(int, int, EBSEventEnum);
    virtual void *AsThrownObject();
    virtual void *AsWeaponObject();
    virtual CTankObject *AsHierTankObject();
    virtual void AttemptScriptMotionUpdate(float);
    virtual void AttemptAnimMotionUpdate(float);
    virtual void AttemptPathMotionUpdate(float);
    virtual void CommitScriptMotionUpdate();
    virtual void CommitAnimMotionUpdate();
    virtual void CommitPathMotionUpdate();
    virtual void CleanUp();
};
class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};
void DestroyBSObject(BSObject *);

void DeleteSpecialThrownObject(CThrownObject *object) {
    ((CThrownObjectInterface *)object)->CleanUp();
    BSObject *script = (BSObject *)((ISceneNode *)object)->GetScriptObject();
    if (script) {
        BSGO_Basic *gameObject = script->gameObject.operator->();
        if (gameObject)
            delete gameObject;
        DestroyBSObject(script);
    }
    CStaticObjectFactory::DeallocateStaticObject((CStaticObject *)object);
}
