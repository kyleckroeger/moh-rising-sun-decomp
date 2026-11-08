// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ScriptTimers.h"
#include "ObjCreate.h"
#include "Toilet.h"
#include <string.h>

extern "C" void *DWI_alloc(const char *, int, int);

CDumpObjectContainer::CDumpObjectContainer() {
    first = last = 0;
    count = 0;
    capacity = 0;
}

CDumpObjectContainer::~CDumpObjectContainer() {
    delete[] objects;
    objects = 0;
}

void CDumpObjectContainer::Reset() {
    if (!objects) {
        LevelFileContentsStruct_ *resource = TLT_FindNextResourceByType((TLTResourceID)27, 0);
        capacity = resource->data[58]->value;
        if (!capacity)
            capacity = 25;
        objects = (DumpObject *)DWI_alloc("source/ai_script/objcreate.cpp:190", capacity * sizeof(DumpObject), 1024);
    }
    memset(objects, 0, capacity * sizeof(DumpObject));
    first = last = 0;
    count = 0;
}

void CDumpObjectContainer::AddDumpObject(CStaticObject *object, const PickupInfo &pickup, bool keep) {
    DumpObject *dump = AllocateDumpObject();
    dump->object = object;
    dump->pickup = pickup;
    dump->keep = keep;
    dump->next = 0;
    dump->previous = last;
    if (last)
        last->next = dump;
    last = dump;
    if (!first)
        first = dump;
    count++;
}

void CDumpObjectContainer::DeleteDumpObject(DumpObject *dump) {
    dump->object = 0;
    if (first == dump) {
        first = dump->next;
        if (last == dump)
            last = 0;
        else
            first->previous = 0;
    } else if (last == dump) {
        last = dump->previous;
        last->next = 0;
    } else {
        dump->next->previous = dump->previous;
        dump->previous->next = dump->next;
    }
    FreeDumpObject(dump);
    count--;
}

DumpObject *CDumpObjectContainer::GetDumpObject(CStaticObject *object) {
    DumpObject *dump = first;
    while (dump->object != object) {
        dump = dump->next;
        if (!dump)
            return 0;
    }
    return dump;
}

DumpObject *CDumpObjectContainer::AllocateDumpObject() {
    DumpObject *dump = GetAvailableObject();
    if (!dump)
        dump = DeleteOldest(true);
    return dump;
}

void CDumpObjectContainer::FreeDumpObject(DumpObject *dump) { dump->inUse = 0; }

DumpObject *CDumpObjectContainer::GetAvailableObject() {
    DumpObject *dump = objects;
    int i;
    for (i = 0; i < capacity; i++, dump++) {
        if (!dump->inUse)
            break;
    }
    if (i == capacity)
        dump = 0;
    if (dump)
        dump->inUse = 1;
    return dump;
}

DumpObject *CDumpObjectContainer::DeleteOldest(bool reuse) {
    DumpObject *dump;
    for (dump = first; dump; dump = dump->next) {
        if (!dump->keep)
            break;
    }
    if (dump) {
        if (dump->pickup.script)
            BSObjectTriggerEvent(dump->pickup.script, 43, 0, 0, true);
        MarkDumpObjectForDestruction(dump);
        if (reuse) {
            dump->inUse = 1;
            return dump;
        }
    }
    return 0;
}

DumpObject *CDumpObjectContainer::GetFirstDumpObject() { return first; }

DumpObject *CDumpObjectContainer::GetNextDumpObject(DumpObject *dump) { return dump->next; }
