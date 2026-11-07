// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
extern "C" int SND3dpos(int, int, int);
int SoundEntry::UpDateSoundParams() {
    int result = 0;
    if (updates & 4) result = SND3dpos(handle, azimuth, 0);
    if ((updates & 2) || (updates & 1)) {
        if (spatial) currentVolume = CalculateVolDistance();
        else currentVolume = volume;
        SetNormSndVol(currentVolume);
    }
    if (updates & 8) SetSndPitch(pitch);
    updates = 0;
    return result;
}
