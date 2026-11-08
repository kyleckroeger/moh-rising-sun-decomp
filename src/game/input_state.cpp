// CInputManager: device readiness, vibration feedback and clearing the event tables.
// SetMasterBlockVibrations, which follows, is not part of this unit.
#include "Shell.h"

bool CInputManager::IsReady(signed char device) const {
    return GetDeviceState(device) != 0;
}

int CInputManager::IsActive(signed char device) const {
    return CDeviceManager::IsActive(device);
}

void CInputManager::SetFeedback(unsigned char player, unsigned char a, unsigned char b, float c, float d) {
    if (!blockVibrations[player] && !masterBlockVibrations[player])
        SetActuatorState(player, a, b, c, d);
}

void CInputManager::Reset() {
    for (int i = 0; i < GetNumDevices(); i++) {
        CInputEvent *table = GetEventTable(i);
        if (table) {
            for (int j = 0; j < 181; j++) {
                table->value = 0;
                table->field04 = 0;
                table++;
            }
        }
    }
}

void CInputManager::SetBlockVibrations(bool block, unsigned char player) {
    blockVibrations[player] = block;
}
