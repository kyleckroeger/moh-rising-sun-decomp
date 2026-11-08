// SNDI_patchtohdr (spat2hdr.c): converts a tagged patch into a sample format, attributes and description. A "GSTR"
// patch goes to SNDI_patchtohdrgen. Otherwise the three records are cleared and given their defaults, then the patch's
// tags are read: format, attribute bytes, copied data blocks, extra blocks, sample addresses, a loop table and per-channel
// pan offsets. Finally, from the last channel, the pan comes from the output layout's channel map plus its offset, the
// address is rebased on the bank for absolute or unflagged samples, and looped samples take their addresses from the
// loop table. SNDTAGINFO, TAGGEDPATCH, SNDSAMPLEFORMAT, SNDSAMPLEATTR and SNDSAMPLEDESC are named by the mangled symbols;
// the members and the tags' meanings are inferred from offsets.
#include "SoundGlobals.h"

struct SNDTAGINFO {
    unsigned char* p;
    int tag;
    int value;
    unsigned char* data;
    unsigned int length;
};

struct TAGGEDPATCH;

namespace Sndgendef {
struct GENTAGGEDPATCH;
}

struct SNDSAMPLEFORMAT {
    unsigned short rate;
    unsigned char channels;
    unsigned char bits;
};

struct SNDSAMPLEATTR {
    unsigned char field00[2];
    unsigned char field02;
    unsigned char field03[2];
    unsigned char field05;
    unsigned char field06;
    unsigned char field07;
    unsigned short flags;
    unsigned char field0a[2];
    unsigned short pan[6];
    void* data[6];
    unsigned int length[6];
    unsigned char* extra[4];
    unsigned int extraLength[4];
};

struct SNDSAMPLEDESC {
    int field00;
    int address[6];
};

extern "C" {
void* memset(void*, int, unsigned long);
void SND_attrsetdef(SNDSAMPLEATTR*);
void SNDI_memcpy(void*, const void*, int);
}

int SNDI_gettag(SNDTAGINFO*);
void* SNDMEMI_allocz(int);
void SNDI_patchtohdrgen(void*, Sndgendef::GENTAGGEDPATCH*, SNDSAMPLEFORMAT*, SNDSAMPLEATTR*, SNDSAMPLEDESC*);

void SNDI_patchtohdr(void* bank, TAGGEDPATCH* patch, SNDSAMPLEFORMAT* format, SNDSAMPLEATTR* attr, SNDSAMPLEDESC* desc, unsigned char* generic) {
    SNDTAGINFO info;
    int offsets[6] = {0};
    int extra = 0;
    unsigned int flags = 512;
    int* loop = 0;
    int i;
    unsigned char* id = (unsigned char*)patch;

    memset(format, 0, 4);
    memset(attr, 0, 104);
    memset(desc, 0, 28);
    if (!(id[0] == 'P' && id[1] == 'T') && id[0] == 'G' && id[1] == 'S' && id[2] == 'T' && id[3] == 'R') {
        SNDI_patchtohdrgen(0, (Sndgendef::GENTAGGEDPATCH*)patch, format, attr, desc);
        if (generic)
            *generic = 1;
        return;
    }
    if (generic)
        *generic = 0;
    SND_attrsetdef(attr);
    info.p = (unsigned char*)patch + 4;
    format->rate = 24000;
    format->channels = 1;
    format->bits = 7;
    desc->field00 = 0;
    while (SNDI_gettag(&info)) {
        if (info.tag == 160)
            format->bits = info.value;
        else if (info.tag == 156)
            offsets[0] = info.value;
        else if (info.tag == 157)
            offsets[1] = info.value;
        else if (info.tag == 158)
            offsets[2] = info.value;
        else if (info.tag == 159)
            offsets[3] = info.value;
        else if (info.tag == 166)
            offsets[4] = info.value;
        else if (info.tag == 167)
            offsets[5] = info.value;
        else if (info.tag == 152) {
            attr->data[0] = SNDMEMI_allocz(info.length);
            SNDI_memcpy(attr->data[0], info.data, info.length);
            attr->length[0] = info.length;
        }
        else if (info.tag == 153) {
            attr->data[1] = SNDMEMI_allocz(info.length);
            SNDI_memcpy(attr->data[1], info.data, info.length);
            attr->length[1] = info.length;
        }
        else if (info.tag == 154) {
            attr->data[2] = SNDMEMI_allocz(info.length);
            SNDI_memcpy(attr->data[2], info.data, info.length);
            attr->length[2] = info.length;
        }
        else if (info.tag == 155) {
            attr->data[3] = SNDMEMI_allocz(info.length);
            SNDI_memcpy(attr->data[3], info.data, info.length);
            attr->length[3] = info.length;
        }
        else if (info.tag == 164) {
            attr->data[4] = SNDMEMI_allocz(info.length);
            SNDI_memcpy(attr->data[4], info.data, info.length);
            attr->length[4] = info.length;
        }
        else if (info.tag == 165) {
            attr->data[5] = SNDMEMI_allocz(info.length);
            SNDI_memcpy(attr->data[5], info.data, info.length);
            attr->length[5] = info.length;
        }
        else if (info.tag == 128)
            attr->field07 = info.value;
        else if (info.tag == 130)
            format->channels = info.value;
        else if (info.tag == 132)
            format->rate = info.value;
        else if (info.tag == 133)
            desc->field00 = info.value;
        else if (info.tag == 138)
            loop = (int*)info.data;
        else if (info.tag == 19)
            attr->field05 = info.value;
        else if (info.tag == 10)
            attr->field06 = info.value;
        else if (info.tag == 6)
            attr->field02 = info.value;
        else if (info.tag == 140)
            flags = info.value;
        else if (info.tag == 136)
            desc->address[0] = info.value;
        else if (info.tag == 137)
            desc->address[1] = info.value;
        else if (info.tag == 148)
            desc->address[2] = info.value;
        else if (info.tag == 149)
            desc->address[3] = info.value;
        else if (info.tag == 162)
            desc->address[4] = info.value;
        else if (info.tag == 163)
            desc->address[5] = info.value;
        else if (info.tag == 20) {
            attr->extra[extra] = info.data;
            attr->extraLength[extra] = info.length;
            extra++;
        }
    }
    attr->flags = flags;
    for (i = format->channels - 1; i >= 0; i--) {
        if (sndgs.channels == 2 && format->channels == 2)
            offsets[i] = 0;
        attr->pan[i] = sndgs.channelmap[format->channels - 1][i] + offsets[i];
        if ((attr->flags & 20) != 0 || attr->flags == 0)
            desc->address[i] = (int)bank + desc->address[i];
        if ((attr->flags & 776) != 0 && loop) {
            if (i == 0)
                desc->address[i] = loop[0];
            else
                desc->address[i] = loop[0] + (desc->address[i] - desc->address[0]);
        }
    }
}
