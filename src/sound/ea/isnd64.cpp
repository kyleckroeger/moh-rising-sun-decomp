// This file group has no name in the original symbols; the file name is descriptive.
// The file ends with the unsigned 64-bit helpers: a 32x32 multiply built from
// 16-bit partial products and a 64/32 shift-subtract division (the high word
// must be below the divisor). SINT64 is named by the mangled symbols; its
// low/high word order is inferred from the stores.
struct SINT64 {
    unsigned int lo;
    unsigned int hi;
};

SINT64 iSNDmulu64(unsigned int a, unsigned int b) {
    SINT64 result;
    unsigned int carry;
    unsigned int al = a & 0xffff;
    unsigned int bl = b & 0xffff;
    unsigned int ah = a >> 16;
    unsigned int bh = b >> 16;
    unsigned int ll = al * bl;
    unsigned int m1 = al * bh;
    unsigned int m2 = ah * bl;
    unsigned int hh = ah * bh;
    carry = ((ll >> 16) + (m1 & 0xffff) + (m2 & 0xffff)) >> 16;
    result.lo = ll + (m1 << 16) + (m2 << 16);
    result.hi = (m1 >> 16) + (m2 >> 16) + hh + carry;
    return result;
}

unsigned int iSNDdivu64(SINT64 value, int divisor) {
    unsigned int lo = value.lo;
    unsigned int rem = value.hi;
    unsigned int quotient = 0;
    int i;
    for (i = 0; i < 32; i++) {
        rem <<= 1;
        rem += lo >> 31;
        quotient <<= 1;
        lo <<= 1;
        if (rem >= (unsigned int)divisor) {
            rem -= divisor;
            quotient++;
        }
    }
    return quotient;
}
