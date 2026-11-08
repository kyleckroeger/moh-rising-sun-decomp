// A fragment of EA's file device (filedev.cpp): closefile closes a host file
// through PCclose (handle type 3) or a disc file through DVDClose (type 2) and
// reports whether the close returned 0. Any other type reports success.
// HANDLE_def and DVDFileInfo are named by the mangled symbols; the handle's
// members are inferred. The host file handle shares its offset with the disc
// file's record.
struct DVDFileInfo;

struct HANDLE_def {
    int type;
    char field04[260];
    void* file;
};

extern "C" {
int PCclose(void*);
int DVDClose(DVDFileInfo* fileInfo);
}

int closefile(HANDLE_def* handle) {
    int result = 0;

    if (handle->type == 3)
        result = PCclose(handle->file);
    else if (handle->type == 2)
        result = DVDClose((DVDFileInfo*)&handle->file);
    return result == 0;
}
