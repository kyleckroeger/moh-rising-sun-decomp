// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void SPlayerStats::Reset() {
    dirty = 0;
    enemiesKilled = 0;
    hits = 0;
    hitsTaken = 0;
    unknown_a = 0;
    unknown_4[0] = 0;
    unknown_4[1] = 0;
    shotLocation[0] = 0;
    shotLocation[3] = 0;
    shotLocation[6] = 0;
    shotLocation[4] = 0;
    shotLocation[1] = 0;
    shotLocation[5] = 0;
    shotLocation[2] = 0;
    for (int i = 0; i < 8; i++) {
        entries[i].value = 0;
        entries[i].count = 0;
    }
    shotsFired = 0;
    levelTimer = 0;
}
