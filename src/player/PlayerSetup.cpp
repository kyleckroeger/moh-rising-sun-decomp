// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "SceneObjects.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((const PlayerWeaponMap *)weaponMap)
#define MUTABLE_WEAPONS ((PlayerWeaponMap *)weaponMap)
#include "AIDoodad.h"
extern CScreen *g_pScreen;
extern CCamera *g_pCamera;
void CPlayerObject::ReInit() {
    MUTABLE_WEAPONS->clear();
    Init();
}
void CPlayerObject::InitAI() {
    CAIPlayerDoodad *doodad = (CAIPlayerDoodad *)aiDoodad;
    doodad->Init(this);
    aiObject = doodad->CAIPlayerDoodad::GetAIObject();
    aiObject->owner = this;
}
CAIObject *CPlayerObject::GetAIObject() { return aiObject; }
void CPlayerObject::CleanUp() { ((CAIPlayerDoodad *)aiDoodad)->CleanUp(); }
void CPlayerObject::InitClass(CScreen *screen, CCamera *camera) {
    g_pScreen = screen;
    g_pCamera = camera;
}
