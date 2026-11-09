// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::SetPrevMissionAndLevel(unsigned int mission, unsigned int level) {
    profile->profiles[profile->profileIndex[0]].prevMission = mission;
    if (IsCoop()) {
        profile->profiles[profile->profileIndex[0]].coopPrevLevel = level;
        profile->profiles[profile->profileIndex[1]].coopPrevLevel = level;
    } else profile->profiles[profile->profileIndex[0]].prevLevel = level;
}
void CGameSettings::SetNextMissionAndLevel(unsigned int mission, unsigned int level, int savePoint) {
    nextMission = mission;
    nextLevel = level;
    profile->saveData.savePointID = savePoint;
}
void CGameSettings::SetCurrentMissionAndLevel(unsigned int mission, unsigned int level) {
    profile->saveData.mission = mission;
    profile->saveData.level = level;
}
