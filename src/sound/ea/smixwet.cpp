// The start of the effect mixer's file (sfxrevc.c): MIX_getwetbuffer returns
// sndfx, the mixer's wet buffer, and remap reads mapchannel, a table of 6 x 6
// x 6 bytes (its size in the symbol table), with three 1-based indices. The
// function and object names come from the symbols; what the indices mean is
// not established.
extern "C" {
extern char sndfx[];
float* MIX_getwetbuffer(void) {
    return (float*)sndfx;
}
}

extern unsigned char mapchannel[6][6][6];

int remap(int a, int b, int c) {
    return mapchannel[a - 1][b - 1][c - 1];
}
