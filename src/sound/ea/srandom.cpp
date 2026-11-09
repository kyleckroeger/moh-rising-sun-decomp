// A fragment of the sound library's random generator (srandom.c): SNDI_randomseed
// adds a value to each of the six original seed words. The state is owned by the
// rest of the file, which is not part of this unit; names come from the symbols.
extern unsigned int SNDIrandseedorig[6];
extern unsigned int SNDIrandseed[6];

void SNDI_randomseed(unsigned int seed) {
    int i;

    for (i = 0; i < 6; i++)
        SNDIrandseed[i] = SNDIrandseedorig[i] + seed;
}
