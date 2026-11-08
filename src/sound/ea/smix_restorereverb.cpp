// A fragment of the mixer effects (sfxrevc.c): MIX_restorereverb clears the mixer's
// reverb word, restores the second-generation effects and clears the reverb flag.
// The function names come from the symbols; the view is inferred.
struct MIXREVERBVIEW {
    unsigned char unknown00[15];
    unsigned char reverbflag;
    int reverb;
};

extern "C" MIXREVERBVIEW sndmix;
void SNDMIXI_restorefx2();

extern "C" void MIX_restorereverb() {
    int zero = 0;

    sndmix.reverb = zero;
    SNDMIXI_restorefx2();
    sndmix.reverbflag = zero;
}
