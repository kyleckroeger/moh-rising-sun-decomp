// AI-assisted reconstruction from GR8E69. Part of the original Moh3.cpp. RestoreCheats re-applies the
// saved cheats when game data is valid and the profile's cheat byte (+0x31664, name inferred) is set.
#include "MohLoader.h"
#include "GameSettings.h"

void RestoreCheats() {
    if (MohLoader::Instance()->gameDataValid) {
        if (*((unsigned char *)g_GameSettings.profile + 0x31664) == 1)
            g_GameSettings.InitCheatsFromOptions();
    }
}
