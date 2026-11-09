// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::ResetCurrentLevelStats() {
    profile->saveData.stats.Reset();
    profile->levelStats.Reset();
}
void CGameSettings::Set_shotsFired(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.shotsFired += count;
}
void CGameSettings::Set_hits(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.hits += count;
}
void CGameSettings::Set_hitsTaken(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.hitsTaken += count;
}
void CGameSettings::Set_enemiesKilled(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.enemiesKilled += count;
}
void CGameSettings::Set_shotLocation(unsigned int count, EShotLocations location, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.shotLocation[location] += count;
}
