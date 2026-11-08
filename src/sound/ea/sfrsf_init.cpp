// The resampling filter's initialisation (sfrsf.c): the state's
// process function is SFILTER_rsf, its input and position words and two
// halfwords are cleared, the resampling routine is rsflc unless the second
// argument is set, and the four history samples are cleared. RSFSTATE and
// rsflc are named by the symbols; the other field meanings are not established.
struct RSFSTATE {
    int (*process)(void*, int, void*, void*, int);
    int field04;
    unsigned char unknown08[20];
    int pitch;
    int field20;
    void (*resample)(int, float*, float*, int*, unsigned int*, int, unsigned int);
    short field28;
    short field2a;
    float history[4];
};

int SFILTER_rsf(void*, int, void*, void*, int);
extern "C" void rsflc(int count, float* in, float* out, int* index, unsigned int* fraction, int stephigh, unsigned int steplow);

void SFILTER_rsfinit(RSFSTATE* state, int, int custom) {
    state->process = SFILTER_rsf;
    state->field04 = 0;
    state->field20 = 0;
    state->field28 = 0;
    state->field2a = 0;
    if (custom == 0)
        state->resample = rsflc;
    for (int i = 0; i < 4; i++)
        state->history[i] = 0.0f;
}
