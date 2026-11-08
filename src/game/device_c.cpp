// AI-assisted reconstruction from GR8E69. The image records no name for this file.
#include "Shell.h"

int CDeviceManager::GetPortNum(unsigned char device) const {
    return *(const unsigned char *)&devices[device];
}
