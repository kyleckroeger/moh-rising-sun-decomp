// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"

float SoundEntry::CalculateVolDistance() {
    float slope = volumeLimit * (attenuation * (1.0f / 30.0f));
    float v;
    if (!(distance <= 0.0f))
        v = volumeLimit - slope * distance;
    else
        v = volume;
    if (!(v < volumeLimit))
        v = volumeLimit;
    return v;
}
