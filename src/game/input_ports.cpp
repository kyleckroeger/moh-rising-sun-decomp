// CInputManager: shutdown, port assignment, key maps and event values.
#include "Shell.h"
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;

void CInputManager::Shutdown() {}

void CInputManager::SetController(unsigned char device, signed char port) {
    controllers[device] = port;
    SetPort(device, port);
}

void CInputManager::SetKeymap(unsigned char player, KEYMAP *map) {
    if (g_bInMultiplayerMode || g_bCoopMultiplayer)
        portKeymaps[player] = map;
    else
        keymap = map;
}

int CInputManager::GetValue(unsigned char device, unsigned short id) const {
    const CInputEvent *table = GetEventTable(device);
    int value;
    if (!table)
        value = 0;
    else
        value = table[id].value;
    return value;
}
