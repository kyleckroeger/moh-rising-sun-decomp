// A fragment of the big-file code (locatbig.cpp): BIG_debuginfo reads an archive's debug block, and BIG_locateentryz finds
// an entry by name (case-insensitively) or by index, returning its name with its offset and size fields read big-endian
// from the entry record, or null with both cleared. The offset field is read as a 64-bit value whose high part (fields
// wider than 4 bytes) is discarded when it is stored. The functions are named by the symbols; the parameter names and
// the entry layout are inferred.
extern "C" {
int BIG_typeofheader(void* header);
int BIG_sizeofheader(void* header);
unsigned int strlen(const char*);
}
int BIG_stricmp(const char*, const char*);

static int BIG_debuginfo(const void* header, int* version, unsigned int* value) {
    int result = 0;
    const unsigned char* end = (const unsigned char*)header + BIG_sizeofheader((void*)header);
    const char* tag = (const char*)end - 8;

    if (((unsigned char)(tag[0] - 'A') <= 25 || (unsigned char)(tag[0] - 'a') <= 25) && (unsigned char)(tag[1] - '0') <= 9 &&
        (unsigned char)(tag[2] - '0') <= 9 && (unsigned char)(tag[3] - '0') <= 9) {
        result = 8;
        if (version)
            *version = (tag[1] - '0') * 100 + (tag[2] - '0') * 10 + (tag[3] - '0');
        if (value) {
            const unsigned char* p = end - 4;

            *value = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
        }
    }
    return result;
}


/* inferred: an entry field of 1 to 4 big-endian bytes, and an offset field of any width */
static inline unsigned int BIG_getsmall(const unsigned char* p, int bytes) {
    unsigned int value;

    if (bytes == 1)
        value = p[0];
    else if (bytes == 2)
        value = p[0] << 8 | p[1];
    else if (bytes == 3)
        value = p[0] << 16 | p[1] << 8 | p[2];
    else if (bytes == 4)
        value = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
    else
        value = 0;
    return value;
}

static inline unsigned long long BIG_getnum(const unsigned char* p, int bytes) {
    if (bytes <= 4)
        return BIG_getsmall(p, bytes);
    return (unsigned long long)BIG_getsmall(p, bytes - 4) << 32 | BIG_getsmall(p + bytes - 4, 4);
}

extern "C" void* BIG_locateentryz(void* header, const char* name, int index, int* offset, int* size) {
    int count = 0;
    int debug = BIG_debuginfo(header, 0, 0);
    const unsigned char* entry = (const unsigned char*)header + 16;
    int offsetBytes = 4;
    int sizeBytes = 4;
    const unsigned char* end = (const unsigned char*)header + BIG_sizeofheader(header) - debug;

    switch (BIG_typeofheader(header)) {
    case 2:
        break;
    case 3:
        offsetBytes = ((const char*)header)[3] - '0';
        sizeBytes = offsetBytes;
        if (sizeBytes > 4)
            sizeBytes = 4;
        break;
    case 1:
        entry = (const unsigned char*)header + 6;
        offsetBytes = 3;
        sizeBytes = 3;
        break;
    }

    if (name) {
        while (entry < end) {
            const char* entryName = (const char*)entry + offsetBytes + sizeBytes;
            if (!BIG_stricmp(entryName, name)) {
                if (offset)
                    *offset = BIG_getnum(entry, offsetBytes);
                if (size)
                    *size = BIG_getsmall(entry + offsetBytes, sizeBytes);
                return (void*)entryName;
            }
            entry = (const unsigned char*)entryName + strlen(entryName) + 1;
        }
    } else {
        while (entry < end) {
            const char* entryName = (const char*)entry + offsetBytes + sizeBytes;
            if (count == index) {
                if (offset)
                    *offset = BIG_getnum(entry, offsetBytes);
                if (size)
                    *size = BIG_getsmall(entry + offsetBytes, sizeBytes);
                return (void*)entryName;
            }
            count++;
            entry = (const unsigned char*)entryName + strlen(entryName) + 1;
        }
    }
    if (offset)
        *offset = 0;
    if (size)
        *size = 0;
    return 0;
}
