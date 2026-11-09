// AI-assisted reconstruction from GR8E69. Part of the original rcmpplayer.cpp. The decoder's allocator
// aligns to 32 bytes and offsets the result by the caller's value.
extern "C" void *DWI_allocalign(const char *, unsigned int, int, int);
extern "C" int DWI_free(void *);

extern int s_iMemClass;

void *RCMPAlloc(const char *label, int size, int alignment, int offset, int flags) {
    char *block;
    if (s_iMemClass)
        block = (char *)DWI_allocalign(label, size, alignment, s_iMemClass | flags);
    else
        block = (char *)DWI_allocalign("source/shell/rcmpplayer.cpp:79", size, 32, 1024);
    return block + offset;
}

// The original tests s_iMemClass and makes the same call on both paths. The if/else with identical
// arms is inferred: it is the plain form that keeps the test the binary performs.
int RCMPFree(void *block) {
    block = (void *)((unsigned int)block & ~31);
    if (s_iMemClass)
        DWI_free(block);
    else
        DWI_free(block);
    return 1;
}
