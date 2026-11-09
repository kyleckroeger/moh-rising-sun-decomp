// CUserMessageTable: unloading and string lookups by index or CRC.
#include "Objectives.h"

void CUserMessageTable::Unload() {
    if (records)
        delete[] records;
    records = 0;
    if (field04)
        field04 = 0;
    if (field08)
        field08 = 0;
    if (crcTable) {
        delete[] crcTable;
        crcTable = 0;
    }
    count = 0;
}

int CUserMessageTable::Loaded() const {
    int loaded = 0;
    if (field04 && field08 && records)
        loaded = 1;
    return loaded;
}

const char *CUserMessageTable::GetString(int index) {
    if (index >= 0 && index < count)
        return records[index].text;
    return 0;
}

int CUserMessageTable::GetIndexFromCRC(unsigned int crc) {
    if (!crcTable)
        return -1;
    for (int i = 0; i < count; i++) {
        if (crcTable[i] == crc)
            return i;
    }
    return -1;
}

const char *CUserMessageTable::GetStringFromCRC(unsigned int crc) {
    int index = GetIndexFromCRC(crc);
    if (index < 0)
        return 0;
    return GetString(index);
}

int CUserMessageTable::GetStringLength(int index) {
    if (index < count)
        return records[index].length;
    return 0;
}
