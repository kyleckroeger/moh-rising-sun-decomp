// The pathx layer: PATHX_igetslot finds a free track slot, PATHX_iloadbank adds a track's sound
// bank from the given data or a loaded file, keeping only a copied header when the bank
// reports it needs just that (SNDbankadd returning 7), PATHX_iloadfile reads a whole file,
// and PATHX_isynctask runs the installed service.
#include "PathxTrack.h"

int PATHX_igetslot() {
    int i;
    for (i = 0; i < 24; i++) {
        if (pathxinfo[i].stream < 0)
            break;
    }
    return i;
}

int PATHX_iloadbank(int track, char* filename, char* data) {
    int bank = -1;
    if (data)
        pathxinfo[track].bankData = data;
    else
        pathxinfo[track].bankData = PATHX_iloadfile(filename);
    if (!pathxinfo[track].bankData)
        return -39;
    if (SNDbankadd(&bank, pathxinfo[track].bankData) == 7) {
        int size = SNDbankheadersize(bank);
        char* header = (char*)MEM_allocz("PATHXbkh", size, 0);
        if (header) {
            SNDbankheadercopy(header, bank);
            MEM_free(pathxinfo[track].bankData);
            pathxinfo[track].bankData = header;
        }
    }
    return bank;
}

char* PATHX_iloadfile(char* filename) {
    int size = FILE_size(filename);
    if (!size)
        return 0;
    char* buffer = (char*)MEM_allocz("PATHXbnk", size, 0);
    int handle;
    FILESYS_opensync(filename, 1, 0, &handle);
    FILESYS_readsync(handle, 0, buffer, size, 0);
    FILESYS_closesync(handle, 0);
    return buffer;
}

int PATHX_isynctask(int, int) {
    if (pathService)
        pathService();
    return 0;
}
