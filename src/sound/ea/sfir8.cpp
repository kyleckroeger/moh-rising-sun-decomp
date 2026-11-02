// Eight-tap symmetric FIR filter shared by the sound filters.
struct SNDFIRSTATE {
    float history[8];
    float coeffs[5];
};

void SNDI_fir8init(SNDFIRSTATE* state) {
    state->history[0] = 0.0f;
    state->history[1] = 0.0f;
    state->history[2] = 0.0f;
    state->history[3] = 0.0f;
    state->history[4] = 0.0f;
    state->history[5] = 0.0f;
    state->history[6] = 0.0f;
    state->history[7] = 0.0f;
}

void SNDI_fir8(SNDFIRSTATE* state, int count, void* source, void* destination) {
    float* in = static_cast<float*>(source);
    float* out = static_cast<float*>(destination);
    for (int n = 0; n < count; n++) {
        out[n] = state->coeffs[0] * (in[n] + state->history[7]) + state->coeffs[1] * (state->history[0] + state->history[6]) +
                 state->coeffs[2] * (state->history[1] + state->history[5]) +
                 state->coeffs[3] * (state->history[2] + state->history[4]) + state->coeffs[4] * state->history[3];
        state->history[7] = state->history[6];
        state->history[6] = state->history[5];
        state->history[5] = state->history[4];
        state->history[4] = state->history[3];
        state->history[3] = state->history[2];
        state->history[2] = state->history[1];
        state->history[1] = state->history[0];
        state->history[0] = in[n];
    }
}
