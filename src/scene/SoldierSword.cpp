// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AITargeting.h"
#include "AIBehavior.h"

namespace EALA {
namespace Character {
class Mesh {
  public:
    void Hide(const char *);
    void Show(const char *);
};
}
}
void CSoldierObject::OnSwordDraw() {
    if (aiObject)
        ((CSwordBehavior *)((char *)aiObject + 3032))->SetSwordDrawn(true);
    (*(EALA::Character::Mesh **)&((CAnimObject *)this)->skeleton[0xc8 - 0x88])->Hide("M008_GMJswordShape");
}

void CSoldierObject::OnSwordSheath() {
    if (aiObject)
        ((CSwordBehavior *)((char *)aiObject + 3032))->SetSwordDrawn(false);
    (*(EALA::Character::Mesh **)&((CAnimObject *)this)->skeleton[0xc8 - 0x88])->Show("M008_GMJswordShape");
}
