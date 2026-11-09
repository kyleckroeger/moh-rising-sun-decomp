// A fragment of the music path player (path.c): PATH_createtrack passes its
// arguments to PATH_createtrackfrombuf with a null buffer. Only the function
// names come from the symbols; the arguments are not otherwise established.
extern "C" {
int PATH_createtrackfrombuf(int a, int b, int c, int d, int e, int f, int g, int h, int i, void* buffer);

int PATH_createtrack(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    return PATH_createtrackfrombuf(a, b, c, d, e, f, g, h, i, 0);
}
}
