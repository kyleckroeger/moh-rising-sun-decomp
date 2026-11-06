// MIX_getwetbuffer: the mixer's wet (effects) buffer, sndfx. MIX_getframe: ask
// a mixer channel's source for its current frame (-1 when it has no frame
// callback). sndmix's and sndfx's layouts are not known; the channel table
// (80-byte entries) is read through inferred offsets.
extern "C" char sndmix[];
extern "C" char sndfx[];

extern "C" int MIX_getframe(int channel) {
    char* entry = *(char**)(sndmix + 472) + channel * 96;
    int (*getframe)(int) = *(int (**)(int))(entry + 68);

    if (getframe)
        return getframe(*(int*)(entry + 72));
    return -1;
}
