// AI-assisted reconstruction from GR8E69. The image records no name for this file. Member names
// are inferred from what these functions do with them.
#include "Toilet.h"

void CRtcFile::Dearchive() {
    if (archive && needsDearchive && dearchived) {
        for (int i = 0; i < archive->entryCount; i++)
            archive->Entry(i).data = (char *)archive + (int)archive->Entry(i).data;
        needsDearchive = 0;
    }
}

int CRtcFile::GetFileSize() const {
    int size = 0;
    if (archive)
        size = archive->fileSize;
    return size;
}
