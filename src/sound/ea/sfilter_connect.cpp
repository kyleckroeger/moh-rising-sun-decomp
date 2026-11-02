// Mixer filter chains: insert a stage by priority, remove one, and connect
// two stages' ports. The stage record is named only through the C API; its
// members are inferred from offsets and are not original.
struct SFILTER {
    char field0[8];
    SFILTER* in[2];
    SFILTER* out[2];
    unsigned short priority;
    unsigned char connected;
};

extern "C" {
void SFILTER_add(SFILTER** list, SFILTER* filter);

void SFILTER_remove(SFILTER** list, SFILTER* filter);

int SFILTER_connect(SFILTER* source, SFILTER* dest, int output, int input) {
    if (output == 1 && input == 1 && !dest->in[0]) {
        source->out[0] = dest;
        dest->in[0] = source;
        dest->connected = 1;
        return 0;
    }
    if (output == 1 && input == 2 && !dest->in[1]) {
        source->out[0] = dest;
        dest->in[1] = source;
        dest->connected = 1;
        return 0;
    }
    if (output == 2 && input == 1 && !dest->in[0]) {
        source->out[1] = dest;
        dest->in[0] = source;
        dest->connected = 2;
        return 0;
    }
    if (output == 2 && input == 2 && !dest->in[1]) {
        source->out[1] = dest;
        dest->in[1] = source;
        dest->connected = 2;
        return 0;
    }
    return -1;
}
}
