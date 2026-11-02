// MEM_move: copy that allows overlap (backwards when the destination starts
// inside the source).
extern "C" {
void MEM_copy(void*, const void*, int);

void MEM_move(char* dest, const char* source, int size) {
    if (dest <= source || dest >= source + size) {
        MEM_copy(dest, source, size);
    } else {
        source += size;
        dest += size;

        while (size--)
            *--dest = *--source;
    }
}
}
