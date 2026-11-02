// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CAnimObject::SetLightVolumeTransitionDuration(float duration) { lightVolumes.transitionDuration = duration; }
int CAnimObject::AttachObject(CStaticObject *, char *, int) { return 0; }
int CAnimObject::DetachObject(char *) { return 0; }
int CAnimObject::GetAttachPointMatrix(char *, CMatrix *) { return 0; }
void CAnimObject::OnSwordDraw() {}
void CAnimObject::OnSwordSheath() {}
void CAnimObject::AttachFloatingBone() {}
void CAnimObject::DetachFloatingBone() {}
float CAnimObject::GetValue(const char *) { return 0.0f; }
int CAnimObject::EvaluateScript(const char *, unsigned int, const char *) { return 0; }
float CAnimObject::FadeValue() const { return 1.0f; }
void CAnimObject::FadeOut(float) {}
void CAnimObject::FadeIn(float) {}
void CAnimObject::PreTransform(const CMatrix &) {}
void CAnimObject::Transform(const CMatrix &) {}
void CAnimObject::Rotate(const CVector3 &, float) {}
void CAnimObject::Pitch(float) {}
void CAnimObject::Roll(float) {}
void CAnimObject::SetBasis(const CVector3 &, const CVector3 &, const CVector3 &) {}
void *CAnimObject::GetSkeleton() { return skeleton; }
