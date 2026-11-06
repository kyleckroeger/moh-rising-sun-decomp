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
void SFILTER_add(SFILTER** list, SFILTER* filter) {
    SFILTER* prev = 0;
    SFILTER* cur;

    for (cur = *list; cur && cur->priority < filter->priority; cur = cur->in[0])
        prev = cur;
    filter->in[0] = cur;
    if (cur)
        cur->out[0] = filter;
    if (!prev) {
        *list = filter;
    } else {
        prev->in[0] = filter;
        filter->out[0] = prev;
    }
}

void SFILTER_remove(SFILTER** list, SFILTER* filter);

int SFILTER_connect(SFILTER* source, SFILTER* dest, int output, int input);
}
