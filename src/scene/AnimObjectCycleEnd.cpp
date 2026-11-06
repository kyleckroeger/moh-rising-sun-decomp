// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp.
#include "SceneObjects.h"

extern int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);

void CAnimObject::OnCycleEnd() {
    BSObjectTriggerEvent((BSObject *)((ISceneNode *)this)->GetScriptObject(), 27, 0, 0, 0);
}
