// EA's time-stretch filter (stretch.c). crossfade blends two frame buffers
// into an output with a weight stepping by 1/n; determineaction picks, from the
// stored error and the ratio, whether to drop or repeat a frame-sized overlap
// (the offset comes from the next byte of the action stream) or to pass the
// frame through; applyaction performs it, filling the output buffer and
// updating the frame counts; transferframes moves pending output to the
// caller; stretchframesneeded counts the input frames needed by simulating
// determineaction, and stretch produces the output, carrying a frame over when
// the last action stretched. SFILTER_timestretchsetratio takes the ratio in
// 1/4096 steps, clamped to 0.5..2; SFILTER_timestretch is the filter's process
// function and SFILTER_timestretchinit reads the header and the voice's packet
// handle. The function names come from the symbols; the state layout is inferred
// and its fields are named by offset or by use.
struct TIMESTRETCHSTATE {
    int (*process)(void*, int, void*, void*, int);
    int field04;
    TIMESTRETCHSTATE* input;
    unsigned char unknown0c[14];
    unsigned char field1a;
    unsigned char field1b;
    unsigned char* m_1c;
    int m_packet;
    float m_24;
    float m_28;
    int m_framesize;
    int m_available;
    int m_carried;
    int m_pending;
    int m_position;
    float m_carry[510];
    float m_output[1];
};

extern "C" void SNDI_memcpy(void*, const void*, int);
extern "C" double fabs(double);

static void crossfade(float* a, float* b, float* out, int n, int shift) {
    float weight;
    float step;
    int i;
    int skip;

    if (shift > 0) {
        weight = 1.0f;
        step = -1.0f / n;
        skip = shift;
    } else {
        skip = -shift;
        step = 1.0f / n;
        weight = 0.0f;
    }
    for (i = skip; i < n; i++) {
        out[i - skip] = weight * a[i] + (1.0f - weight) * a[i - skip];
        weight += step;
    }
    for (i = 0; i < skip; i++) {
        out[n - skip + i] = weight * b[i] + (1.0f - weight) * a[n - skip + i];
        weight += step;
    }
}

static int determineaction(int framesize, float ratio, unsigned char** pointer, float* value) {
    int offset = **pointer + framesize / 2;
    float a;
    float b;

    if (!(ratio >= 1.0f)) {
        b = ((float)framesize + *value) - ratio * (float)framesize;
        a = ((float)(framesize * 2 - offset) + *value) - (float)(ratio + ratio) * (float)framesize;
        if (__builtin_fabsf(b) < __builtin_fabsf(a)) {
            (*pointer)++;
            *value = b;
            return 0;
        }
        *pointer += 2;
        *value = a;
        return -offset;
    }
    b = ((float)framesize + *value) - ratio * (float)framesize;
    a = ((float)(framesize + offset) + *value) - ratio * (float)framesize;
    if (__builtin_fabsf(b) < __builtin_fabsf(a)) {
        (*pointer)++;
        *value = b;
        return 0;
    }
    (*pointer)++;
    *value = a;
    return offset;
}

static void applyaction(TIMESTRETCHSTATE* state, float** in, int action) {
    float* first;
    float* second;

    if (state->m_carried > 0) {
        first = state->m_carry;
        second = *in;
    } else {
        first = *in;
        second = first + state->m_framesize;
    }
    if (action == 0) {
        SNDI_memcpy(state->m_output, first, state->m_framesize * sizeof(float));
        state->m_available -= state->m_framesize;
        state->m_pending = state->m_framesize;
        *in = second;
    } else if (action > 0) {
        SNDI_memcpy(state->m_output, first, action * sizeof(float));
        crossfade(first, second, &state->m_output[action], state->m_framesize, action);
        state->m_available -= state->m_framesize;
        state->m_pending = state->m_framesize + action;
        *in = second;
    } else {
        crossfade(first, second, state->m_output, state->m_framesize, action);
        SNDI_memcpy(&state->m_output[state->m_framesize], second - action, (state->m_framesize + action) * sizeof(float));
        state->m_available -= state->m_framesize * 2;
        state->m_pending = state->m_framesize * 2 + action;
        *in = second + state->m_framesize;
    }
    state->m_carried = 0;
    state->m_position = 0;
}


extern "C" void SNDI_memcpy(void*, const void*, int);
static int transferframes(TIMESTRETCHSTATE* state, int* frames, float** out) {
    float* source = &state->m_output[state->m_position];
    float* dest = *out;
    int count;

    if (*frames <= state->m_pending)
        count = *frames;
    else
        count = state->m_pending;

    SNDI_memcpy(dest, source, count * sizeof(float));
    state->m_position += count;
    state->m_pending -= count;
    *frames -= count;
    *out += count;
    return count;
}

extern "C" void* memset(void*, int, unsigned long);
extern "C" int SNDPKTPLAY_framesoutstanding(int);
int SNDDRV_getmastervoice(int);
int SNDPKTPLAYI_voicetopackethandle(int);
int SNDI_getb(void*, int);

int stretchframesneeded(void* handle, int frames) {
    TIMESTRETCHSTATE* state = (TIMESTRETCHSTATE*)handle;
    int action;
    int produced;
    int consumed;
    int needed;
    int available;
    unsigned char* pointer;
    float value;

    if (frames > state->m_pending) {
    pointer = state->m_1c;
    needed = 0;
    value = state->m_28;
    action = 0;
    frames -= state->m_pending;
    available = state->m_available;
    while (frames > 0) {
        if (available < state->m_framesize)
            break;
        if (available < state->m_framesize * 2)
            action = 0;
        else
            action = determineaction(state->m_framesize, state->m_24, &pointer, &value);
        if (action >= 0) {
            consumed = state->m_framesize;
            produced = consumed + action;
        } else {
            consumed = state->m_framesize * 2;
            produced = consumed + action;
        }
        needed += consumed;
        available -= consumed;
        if (frames <= produced)
            frames = 0;
        else
            frames -= produced;
    }
    if (action > 0)
        needed += state->m_framesize;
    if (frames > 0) {
        if (frames > available)
            needed += available;
        else
            needed += frames;
    }
    if (needed > state->m_carried)
        return needed - state->m_carried;
    }
    return 0;
}

int stretch(void* handle, int frames, float* in, float* out) {
    TIMESTRETCHSTATE* state = (TIMESTRETCHSTATE*)handle;
    int total = transferframes(state, &frames, &out);
    int action;

    if (frames == 0)
        return total;
    action = 0;
    while (frames > 0) {
        if (state->m_available < state->m_framesize)
            break;
        if (state->m_available < state->m_framesize * 2)
            action = 0;
        else
            action = determineaction(state->m_framesize, state->m_24, &state->m_1c, &state->m_28);
        applyaction(state, &in, action);
        total += transferframes(state, &frames, &out);
    }
    if (action > 0) {
        SNDI_memcpy(state->m_carry, in, state->m_framesize * sizeof(float));
        state->m_carried = state->m_framesize;
        in += state->m_framesize;
    }
    if (frames > 0) {
        int count;

        if (frames > state->m_available)
            count = state->m_available;
        else
            count = frames;
        SNDI_memcpy(out, in, count * sizeof(float));
        total += count;
        state->m_available -= count;
    }
    return total;
}

void SFILTER_timestretchsetratio(TIMESTRETCHSTATE* state, int ratio) {
    float value = (float)ratio * 0.00024414062f;

    state->m_24 = value;
    if (!(value >= 0.5))
        state->m_24 = 0.5f;
    else if (!(value <= 2.0))
        state->m_24 = 2.0f;
}

int SFILTER_timestretch(void* filter, int count, void* source, void* destination, int) {
    TIMESTRETCHSTATE* state = (TIMESTRETCHSTATE*)filter;
    int needed;
    float* in = (float*)source;
    float* out = (float*)destination;
    int done;

    needed = stretchframesneeded(state, count);
    if (state->m_packet >= 0 && needed > SNDPKTPLAY_framesoutstanding(state->m_packet))
        return 0;
    if (needed > 0) {
        int result = state->input->process(state->input, needed, out, in, state->field1a);
        if (result <= 0)
            return result;
    }
    done = stretch(state, count, in, out);
    if (done < count) {
        out += done;
        memset(out, 0, (count - done) * sizeof(float));
    }
    return count;
}

int SFILTER_timestretchinit(TIMESTRETCHSTATE* state, unsigned char* header, int voice) {
    state->process = SFILTER_timestretch;
    state->field04 = 0;
    if (voice >= 0)
        state->m_packet = SNDPKTPLAYI_voicetopackethandle(SNDDRV_getmastervoice(voice));
    else
        state->m_packet = -1;
    state->m_1c = header + 6;
    state->m_24 = 1.0f;
    state->m_28 = 0.0f;
    state->m_framesize = header[1] * 2;
    state->m_available = SNDI_getb(header + 2, 4);
    state->m_carried = 0;
    state->m_pending = 0;
    state->m_position = 0;
    return 0;
}
