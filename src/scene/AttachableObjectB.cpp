// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"
void CAttachableObject::Draw(CDrawContext &context) { ((CStaticObject *)this)->Draw(context); }
int CAttachableObject::IsVisible(CDrawContext &context) const { return ((const CStaticObject *)this)->IsVisible(context); }
