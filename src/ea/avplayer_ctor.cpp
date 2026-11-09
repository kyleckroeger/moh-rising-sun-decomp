// RCMP::AV_PLAYER's constructor: the streamer starts with no player and Init does the rest.
#include "AvPlayer.h"

namespace RCMP {

AV_PLAYER::AV_PLAYER(const char* video, int videoSize, const char* audio, int audioSize, LOAD_ENUM load, SOUND_ENUM sound) {
    Init(video, 0, videoSize, audio, 0, audioSize, load, sound);
}

}
