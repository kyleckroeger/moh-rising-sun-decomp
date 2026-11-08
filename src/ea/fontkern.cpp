// fontkern.cpp, the whole file: FONT_getkern, the kerning between a character and the next one. A font
// either stores each character's kerning pairs in its own list (found
// through the character's kerning index) or keeps one table of pairs keyed
// by both characters. FONT_getkern is named by its symbol; the font,
// character and kerning-pair layouts and the lookups' parameters are inferred.
struct FONTKERN {
    unsigned short next;
    signed char amount;
    unsigned char first;
};

struct FONTCHARVIEW {
    unsigned short code;
    unsigned char unknown02[9];
    unsigned char kernCount;
    unsigned short kernIndex;
};

struct FONTVIEW {
    unsigned char unknown00[12];
    unsigned int flags;
    unsigned char unknown10[8];
    int kernOffset;
};

static FONTKERN* FONT_findkern16(int count, const FONTKERN* pairs, int next) {
    for (int i = 0; i < count; i++) {
        if (pairs[i].next == next)
            return (FONTKERN*)&pairs[i];
    }
    return 0;
}

static FONTKERN* FONT_findkern12(int count, const FONTKERN* pairs, int next, int first) {
    for (int i = 0; i < count; i++) {
        if (pairs[i].next == next && pairs[i].first == first)
            return (FONTKERN*)&pairs[i];
    }
    return 0;
}

extern "C" int FONT_getkern(FONTVIEW* font, FONTCHARVIEW* ch, int next) {
    int kern = 0;
    int count = ch->kernCount;

    if (count) {
        FONTKERN* pair;

        if (font->flags & 0x40000) {
            pair = FONT_findkern16(count, (FONTKERN*)((char*)font + font->kernOffset + (ch->kernIndex * 4 + 4)), next);
        } else {
            int* table = (int*)((char*)font + font->kernOffset);

            pair = FONT_findkern12(*table, (FONTKERN*)(table + 1), next, ch->code);
        }
        if (pair)
            kern = pair->amount;
    }
    return kern;
}
