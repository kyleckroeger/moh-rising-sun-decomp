// FEListBox: rows and columns of cells.
#include "FEListBox.h"
#include <string.h>

void FEListBox::SetAutoWrap(bool wrap) {
    if (wrap) flags88 &= ~4;
    else flags88 |= 4;
}
void FEListBox::InitializeListEntry(FEListEntryData *entry, unsigned long count) {
    void *data = entry;
    memset(data, 0, count * sizeof(FEListEntryData));
}
