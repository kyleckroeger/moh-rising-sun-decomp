/* A fragment of the sound driver (snddrv.c): SNDPLATFORM_asyncresolvetimbre resolves a
   timbre's sample data asynchronously (the data offset is taken from the header when none
   is given, and the sample pointer becomes the buffer plus the remaining offset). The
   patch header's members are inferred. */
struct SNDIPATCHHEADER {
    unsigned char unknown00[3];
    signed char channels;
    unsigned char unknown04[15];
    unsigned char mixmode;
    unsigned char unknown14[66];
    unsigned short rate;
    unsigned int length;
    unsigned int loopStart;
    int loopEnd;
    unsigned char** data;
    int offset;
    int offsets[4];
    int extra[8];
    unsigned char* coefs[4];
    unsigned short* loopContext[4];
};

int SNDPLATFORM_asyncresolvetimbre(SNDIPATCHHEADER* header, char* buffer, int* offset) {
    if (*offset == 0)
        *offset = header->offset;
    *header->data = (unsigned char*)(buffer + (header->offset - *offset));
    return 0;
}
