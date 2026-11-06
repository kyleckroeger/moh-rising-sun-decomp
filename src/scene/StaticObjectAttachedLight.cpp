// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"

// The light reference at +0x294 is an observer whose subject at +0x14 is the light.
struct AttachedLightRef : public IObserver {
    operator bool() const { return subject != 0; }
    CLight *Light() const { return (CLight *)subject; }
};

void CStaticObject::SetAttachedLight(CLight *light, const CVector3 &offset) {
    AttachedLightRef &ref = *(AttachedLightRef *)unknown_294;
    ref.Assign((ISubject *)light);
    if (ref) {
        attachedLightOffset = offset;
        CMatrix matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        ((ISceneNode *)this)->GetTMLocalToWorld(matrix);
        matrix.PreTranslate(attachedLightOffset);
        ((IMovingSceneNode *)ref.Light())->SetTMLocalToWorld(matrix);
    }
}
