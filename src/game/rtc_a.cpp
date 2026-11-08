// AI-assisted reconstruction from GR8E69. The image records no name for this file. Member names
// are inferred from what these functions do with them.
#include "Toilet.h"

void CRtcFile::SetArchiveBuffer(char *buffer, void (*function)(void *)) {
    if (!archive) {
        archive = (RtcArchive *)buffer;
        needsDearchive = 1;
        freeFunction = function;
        dearchived = 1;
    }
}

void CRtcFile::DeallocateArchiveBuffer() {
    if (archive) {
        if (!dearchived) {
            for (int i = 0; i < archive->entryCount; i++) {
                if (archive->Entry(i).data)
                    delete[] archive->Entry(i).data;
            }
        }
        if (freeFunction) {
            freeFunction(archive);
            freeFunction = 0;
        } else if (archive) {
            delete[] (char *)archive;
        }
        archive = 0;
    }
}

CRtcFile::~CRtcFile() {
    DeallocateArchiveBuffer();
}
