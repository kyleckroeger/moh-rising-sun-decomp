/* EA's linear-interpolation resampling loop (scrsfl.c, the whole file):
   rsflc fills count output samples from the input at a 16.16 fixed-point
   position (whole samples in *index, the fraction kept in the high half of
   *fraction between calls) advancing by a 16.16 step built from two
   arguments, interpolating each output between two neighbouring inputs.
   The name comes from the symbol; the parameter roles are inferred. */
extern "C" void rsflc(int count, float* in, float* out, int* index, unsigned int* fraction, int stephigh,
                      unsigned int steplow) {
    int i;
    unsigned int step;

    *fraction >>= 16;
    step = stephigh << 16 | steplow >> 16;
    for (i = 0; i < count; i++) {
        out[i] = in[*index] + 1.5258e-05f * (float)*fraction * (in[*index + 1] - in[*index]);
        *fraction += step;
        *index += *fraction >> 16;
        *fraction &= 0xFFFF;
    }
    *fraction <<= 16;
}
