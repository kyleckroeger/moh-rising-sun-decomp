// FEListBox: rows and columns of cells.
#include "FEListBox.h"
#include <string.h>

FEListBox::~FEListBox() { Terminate(); }
void FEListBox::Terminate() {
    if (((flags88 ^ 1) & 1) == 0) {
        flags88 &= ~1;
        CleanupColumns();
        CleanupRows();
        CleanupCells();
    }
}
