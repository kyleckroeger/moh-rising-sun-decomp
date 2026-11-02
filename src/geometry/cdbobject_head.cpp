// Fragment of the CDB object unit: the constructor and the pass-through switch.
#include "CDBObject.h"

extern bool g_bDisablePassThru;

CCDBObject::CCDBObject(CDB *cdb) : database(cdb) {}

void CCDBObject::DisablePassThru(bool disable) { g_bDisablePassThru = disable; }
