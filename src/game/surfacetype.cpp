// CSurfaceType: per-surface data from the collision databases' surface tables.
// Which of the two databases is read depends on the second argument.
#include "CDBObject.h"

// A 12-byte surface record. The flag and field names follow the accessors.
struct SurfaceTypeRecord {
    unsigned short flags; // bit 0 shoot through, 1 pass through, 2 see through, 3 climbable
    unsigned short soundIndex;
    unsigned int decalCRC;
    unsigned int particleCRC;
};

extern const CDB *g_pCDB1;
extern const CDB *g_pCDB2;

void CSurfaceType::InitClass(const CDB *first, const CDB *second) {
    g_pCDB1 = first;
    g_pCDB2 = second;
}

static inline const SurfaceTypeRecord *Records(const CDB *cdb) {
    return (const SurfaceTypeRecord *)cdb->surfaceTypes;
}

#define SURFACE_DB (second ? g_pCDB2 : g_pCDB1)

unsigned int CSurfaceType::GetDecalCRC(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return 0;
    return Records(cdb)[type].decalCRC;
}

unsigned int CSurfaceType::GetParticleCRC(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return 0;
    return Records(cdb)[type].particleCRC;
}

int CSurfaceType::GetSoundIndex(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return 0;
    return Records(cdb)[type].soundIndex;
}

bool CSurfaceType::IsShootThru(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return false;
    return Records(cdb)[type].flags & 1;
}

bool CSurfaceType::IsPassThru(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return false;
    return (Records(cdb)[type].flags >> 1) & 1;
}

bool CSurfaceType::IsSeeThru(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return false;
    return (Records(cdb)[type].flags >> 2) & 1;
}

bool CSurfaceType::IsClimbable(unsigned short type, bool second) {
    const CDB *cdb = SURFACE_DB;
    if (!cdb || !cdb->surfaceTypeCount)
        return false;
    return (Records(cdb)[type].flags >> 3) & 1;
}
