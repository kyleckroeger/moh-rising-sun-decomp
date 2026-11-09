// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp.
#include "SceneObjects.h"

class CScreen;
class CCharacterShadow {
  public:
    static void InitClass();
};
void CAnimObject::InitClass(CScreen *) { CCharacterShadow::InitClass(); }
