// A fragment of the music path player (path.c): PATH_overheadtrack sizes a
// track for a number of nodes: PATHX_overheadtrack's size, a 220-byte record
// and 16 bytes per node, then taken up to the next multiple of 16. The function
// name comes from the symbol; the meaning of the sizes is inferred.
extern "C" {
int PATHX_overheadtrack(int nodes);

int PATH_overheadtrack(int nodes) {
    int size = PATHX_overheadtrack(nodes) + 220;

    size += nodes * 16;
    size += 16 - size % 16;
    return size;
}
}
