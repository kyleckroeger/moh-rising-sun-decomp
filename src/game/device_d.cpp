// AI-assisted reconstruction from GR8E69. The image records no name for this file. Class and
// method names come from the symbol table. Reads and writes of the file-local port state tables
// (IsActive, GetCurrentState, the constructor and Update) remain original.
#include "Shell.h"

void CDeviceManager::UpdateActuators(float elapsed) {
    for (int i = 0; i < numDevices; i++)
        devices[i].UpdateActuators(elapsed);
}

int CDeviceManager::SetActuatorState(unsigned char device, unsigned char a, unsigned char b, float c, float d) {
    devices[device].SetActuatorFX(a, b, c, d);
    return 1;
}

CInputEvent *CDeviceManager::GetEventTable(unsigned char device) {
    return devices[device].GetEventTable();
}

const CInputEvent *CDeviceManager::GetEventTable(unsigned char device) const {
    return devices[device].GetEventTable();
}
