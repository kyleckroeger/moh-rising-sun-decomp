// AI-assisted reconstruction from GR8E69. Part of the original Moh3.cpp. The two persistent block
// constants and the three default settings bytes are as stored; their meanings are not recovered.
#include "MohLoader.h"
#include <string.h>

extern "C" {
int DVDSetAutoFatalMessaging(int enable);
unsigned int VIGetTvFormat();
int OSGetResetButtonState();
void VIWaitForRetrace();
void VISetBlack(int black);
void VIFlush();
void OSResetSystem(int reset, unsigned int resetCode, int forceMenu);
}

MohLoader::MohLoader(bool valid) {
    tvFormat = 0;
    unknown58 = 0;
    gameDataValid = valid;
    resetEnabled = 1;
    if (!valid) {
        memset(GameData(), 0, 0x321ec);
        ((char *)GameData())[0x31664] = 1;
        ((char *)GameData())[0x31665] = 16;
        ((char *)GameData())[0x3201c] = 1;
    }
}

MohLoader *MohLoader::Instance() {
    return MohLoaderGc::InstanceGc();
}

void *MohLoader::GameData() {
    return MohLoaderGc::GameDataGc();
}

void MohLoaderGc::SetReturningFlag() {
    g_pPersistent->returningMagic = 0xc0de0bac;
}

void MohLoaderGc::ClearReturningFlag() {
    g_pPersistent->returningMagic = 0;
}

MohLoaderGc *MohLoaderGc::InstanceGc() {
    if (!m_pLoaderGc) {
        if (g_pPersistent->returningMagic != 0xc0de0bac) {
            g_pPersistent->gameDataMagic = 0;
            g_pPersistent->resetButtonPressed = 0;
        }
        bool valid = g_pPersistent->gameDataMagic == 0xc0de0101;
        m_pLoaderGc = new MohLoaderGc(valid);
    }
    return m_pLoaderGc;
}

MohLoaderGc::MohLoaderGc(bool valid) : MohLoader(valid) {
    resetRequested = 0;
}

void MohLoaderGc::Initialize() {
    DVDSetAutoFatalMessaging(1);
    tvFormat = VIGetTvFormat();
    OSGetResetButtonState();
    VIWaitForRetrace();
    OSGetResetButtonState();
    VIWaitForRetrace();
}

void MohLoaderGc::InitVideo() {
}

void *MohLoaderGc::GameDataGc() {
    return g_pPersistent->gameData;
}

void MohLoaderGc::ValidateGameData() {
    g_pPersistent->gameDataMagic = 0xc0de0101;
}

void MohLoaderGc::InvalidateGameData() {
    g_pPersistent->gameDataMagic = 0;
}

void MohLoaderGc::RequestReset() {
    resetRequested = 1;
}

int MohLoaderGc::WasResetButtonPressed() {
    return g_pPersistent->resetButtonPressed;
}

void MohLoaderGc::ResetButtonPressed(bool pressed) {
    g_pPersistent->resetButtonPressed = pressed;
}

bool MohLoaderGc::IsResetConsoleRequested() {
    if (OSGetResetButtonState()) {
        if (!WasResetButtonPressed())
            ResetButtonPressed(true);
    } else if (WasResetButtonPressed()) {
        ResetButtonPressed(false);
        InvalidateGameData();
        RequestReset();
    }
    CheckForControllerReset();
    return resetRequested && resetEnabled;
}

void MohLoaderGc::ResetConsole() {
    VISetBlack(1);
    VIFlush();
    VIWaitForRetrace();
    OSResetSystem(0, 0x80000000, 0);
}
