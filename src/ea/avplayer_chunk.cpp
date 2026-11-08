// RCMP::AV_PLAYER's decoder callbacks and ReleaseRCMPChunk, which returns a chunk to the stream.
#include "AvPlayer.h"

extern "C" void STREAM_release(int handle, unsigned char* chunk);

namespace RCMP {

void AV_PLAYER::StaticGetRCMPChunk(DECODER* decoder, STREAMER* streamer, CHUNK** chunk) {
    streamer->player->GetRCMPChunk(decoder, chunk);
}

void AV_PLAYER::ReleaseRCMPChunk(CHUNK* chunk) {
    STREAM_release(m_stream, (unsigned char*)chunk->data);
    delete chunk;
}

void AV_PLAYER::StaticReleaseRCMPChunk(DECODER*, STREAMER* streamer, CHUNK* chunk) {
    streamer->player->ReleaseRCMPChunk(chunk);
}

}
