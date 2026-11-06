// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "EAGLTransform.h"
#include "ScriptRuntime.h"

class CAnimObjectFactory {
  public:
    void ReleaseObject(CAnimObject *);
};
extern CAnimObjectFactory g_AnimObjFactory;

unsigned int CAnimObject::IsDrawEnabled() const { return 1; }
EClsnId CAnimObject::GetCollisionId() const { return (EClsnId)collisionId; }

void CAnimObject::Reset() {
    transform.GetSlot(0);
    hurtling = 0;
    yaw = 0.0f;
}

void CAnimObject::SetPosition(const CVector3 &position) { transform.SetPos(position); }
void CAnimObject::Move(const CVector3 &offset) { transform.Translate(offset); }
