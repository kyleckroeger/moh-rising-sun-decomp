// The sound system's options (ssysinit.c): SNDSYS_getopts asks the platform for
// its output capabilities once, setting three default options and keeping a
// copy of the options as applied, and hands the caller the global record's first
// 212 bytes with the capability result. SNDSYS_setopts copies the caller's
// options in, applies the output settings and keeps the applied copy. The
// function names and gotcaps come from the symbols; the record's division into
// blocks is inferred from the copies, and the member names are not original.

/* inferred: the options block (160 bytes) */
struct SNDSYSOPTS {
    int words[40];
};

/* inferred: the 20 bytes that follow it */
struct SNDSYSEXTRA {
    int words[5];
};

/* inferred: the part of the global record the options occupy */
struct SNDSYSRECORD {
    unsigned char header00[32];
    SNDSYSOPTS opts;
    SNDSYSEXTRA extra;
    SNDSYSOPTS applied;
};

/* inferred: the global record's first 212 bytes, as the caller receives them */
struct SNDSYSRECORDHEAD {
    int words[53];
};

/* inferred: the option members set here */
struct SNDSYSDEFAULTS {
    unsigned char unknown00[40];
    unsigned short bankcount;
    unsigned char unknown2a[10];
    float rate;
    unsigned char unknown38[4];
    unsigned char field3c;
    unsigned char unknown3d[14];
    unsigned char field4b;
};

extern "C" {
extern SNDSYSRECORD sndgs;
}
int SNDPLATFORM_outputcaps();
void SNDPLATFORM_outputset();

static int gotcaps = 0;

extern "C" int SNDSYS_getopts(SNDSYSRECORDHEAD* out) {
    static int ret = 0;

    if (!gotcaps) {
        SNDSYSDEFAULTS* defaults;

        ret = SNDPLATFORM_outputcaps();
        gotcaps = 1;
        defaults = (SNDSYSDEFAULTS*)&sndgs;
        defaults->bankcount = 16;
        defaults->field4b = 90;
        defaults->field3c = 1;
        defaults->rate = -1.0f;
        sndgs.applied = sndgs.opts;
    }
    *out = *(SNDSYSRECORDHEAD*)&sndgs;
    return ret;
}

extern "C" int SNDSYS_setopts(SNDSYSRECORD* options) {
    sndgs.opts = options->opts;
    sndgs.extra = options->extra;
    SNDPLATFORM_outputset();
    sndgs.applied = sndgs.opts;
    return 0;
}
