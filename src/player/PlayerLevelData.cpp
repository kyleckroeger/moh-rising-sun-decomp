// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "GameSettings.h"
#include "ScriptRuntime.h"
void CPlayerObject::SetFinishedLevelData() {
    GameProfile *profile = g_GameSettings.profile;
    if (((GameProfileLevelFlags *)&profile->mission)->levelWon) {
        int mission = profile->mission;
        int level = profile->level;
        g_GameSettings.SetPlayersFinishedLevelSuccessfully(mission, level);
        g_GameSettings.SetPlayersFinishedLevelHealth(mission, level, *(float *)script->gameObject->GetScriptData());
    }
}
