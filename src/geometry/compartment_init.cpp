// Fragment of compartment.cpp: Init with its tables and collision database, InitCDB and
// GlobalReset.
#include "Compartment.h"

void CCompartment::Init(int newIndex, char *newName, CPTHeader *table, CDB *cdb, CRtcFile *file) {
    index = newIndex;
    name = newName;
    primerTable = table;
    database = cdb;
    Init(file);
    collisionVolume.Init(database);
}

void CCompartment::InitCDB(int, char *, CDB *cdb) {
    database = cdb;
    collisionVolume.Init(database);
}

void CCompartment::GlobalReset() {}
