// CUserMessageTable's constructor and destructor.
#include "Objectives.h"

CUserMessageTable::CUserMessageTable() {
    records = 0;
    field08 = 0;
    field04 = 0;
    crcTable = 0;
    count = 0;
}

CUserMessageTable::~CUserMessageTable() {
    Unload();
}
