// CInputManager: locking a single player to a port and finding a device by port.
#include "Shell.h"

class ShellManager {
  public:
    void SetNumberJoyPads(unsigned char);
};
extern ShellManager g_Shell;

void CInputManager::SetSPLockedPort(unsigned char port) {
    SetController(0, port);
    for (int i = 1; i <= 3; i++)
        SetController(i, -1);
    g_Shell.SetNumberJoyPads(1);
}

int CInputManager::FindDeviceIDFromPortNum(unsigned char port) const {
    for (int i = 0; i < GetNumDevices(); i++) {
        if (GetPortNum(i) == port)
            return (signed char)i;
    }
    return -1;
}

void CInputManager::SetBadControllerPort(unsigned char port) {
    if (GetCurrentState(port) != 2) {
        badControllerPort = port;
        badControllerSet = 1;
    }
}
