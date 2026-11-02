// The one-pole (RC) low-pass filter. Field meanings beyond the code are not
// established.
struct LPFRCSTATE {
    int (*process)(void*, int, void*, void*, int);
    int field04;
    LPFRCSTATE* input;
    int field0c;
    int field10;
    int field14;
    unsigned char unknown18[2];
    unsigned char field1a;
    unsigned char field1b;
    float output;
    float feedback;
    float gain;
};

int SFILTER_lpfRC(void* filter, int count, void* source, void* destination, int) {
    LPFRCSTATE* state = static_cast<LPFRCSTATE*>(filter);
    if (state->input) {
        int result = state->input->process(state->input, count, destination, source, state->field1a);
        if (result <= 0)
            return result;
    }
    float* in = static_cast<float*>(source);
    float* out = static_cast<float*>(destination);
    float* values = &state->output;
    for (int i = 0; i < count; i++) {
        values[0] = values[0] * values[1] + values[2] * in[i];
        out[i] = values[0];
    }
    return count;
}

int SFILTER_createLPFRC(LPFRCSTATE* state);

void SFILTER_modifyLPFRC(LPFRCSTATE* state, int* parameters);
