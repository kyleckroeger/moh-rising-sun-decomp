// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "ScriptRuntime.h"
struct BS_STRUCT_HitByBulletInfo_struct {
    BSObject *source;
    float damage;
    int front;
    int bulletType;
    int location;
    int unknown_14[3];
};
void CPlayerObject::SetMPShotLocation(BS_STRUCT_HitByBulletInfo_struct *info) {
    if (info->damage != 0.0f && !(*(float *)script->gameObject->GetScriptData() <= 0.0f)) {
        int location;
        switch (info->location) {
        case 3:
        case 4:
        case 5:
            location = 1;
            break;
        case 6:
        case 7:
        case 8:
            location = 2;
            break;
        case 13:
        case 14:
        case 15:
            location = 4;
            break;
        case 16:
        case 17:
        case 18:
            location = 5;
            break;
        case 19:
        case 20:
            location = 6;
            break;
        case 0:
        case 1:
            location = 0;
            break;
        case 2:
        case 9:
        case 10:
        case 11:
        case 12:
            location = 3;
            break;
        default:
            location = 3;
            break;
        }
    }
}
