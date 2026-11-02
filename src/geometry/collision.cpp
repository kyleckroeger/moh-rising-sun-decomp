// Most of collision.cpp: CCollision construction, assignment, merging, contact storage and
// the two-volume test driver. The driver pads both volumes' radii by LARGEST_MOVE_PER_TICK
// while testing.
#include "Volume.h"
#include "SceneNode.h"

CCollision::CCollision()
    : first(0), second(0), unknown_08(-1.0f), depth(1.0f), lineTest(0), unknown_10a(0), unknown_10b(0), unknown_10c(0),
      unknown_10d(0), separationOffset(0.0f), result(0), contacts(0), contactCount(0) {}

CCollision::CCollision(const CCollision &other)
    : first(other.first), second(other.second), unknown_08(other.unknown_08), depth(other.depth),
      lineTest(other.lineTest), unknown_10a(other.unknown_10a), unknown_10b(other.unknown_10b),
      unknown_10c(other.unknown_10c), unknown_10d(other.unknown_10d), separationOffset(0.0f), result(other.result),
      contacts(0), contactCount(0) {
    for (const SClsnContact *contact = other.contacts; contact; contact = contact->next)
        AddContact(*contact);
    separationOffset = other.separationOffset;
}

CCollision::~CCollision() {
    while (contacts) {
        SClsnContact *next = contacts->next;
        ms_contactPool.Free(contacts);
        contacts = next;
        contactCount--;
    }
}

CCollision &CCollision::operator=(const CCollision &other) {
    if (&other != this) {
        while (contacts) {
            SClsnContact *next = contacts->next;
            ms_contactPool.Free(contacts);
            contacts = next;
        }
        contacts = 0;
        contactCount = 0;
        first = other.first;
        second = other.second;
        unknown_08 = other.unknown_08;
        depth = other.depth;
        lineTest = other.lineTest;
        unknown_10a = other.unknown_10a;
        unknown_10b = other.unknown_10b;
        unknown_10c = other.unknown_10c;
        unknown_10d = other.unknown_10d;
        separationOffset = 0.0f;
        result = other.result;
        for (const SClsnContact *contact = other.contacts; contact; contact = contact->next)
            AddContact(*contact);
        separationOffset = other.separationOffset;
    }
    return *this;
}

CCollision &CCollision::operator+=(const CCollision &other) {
    result = other.result;
    if (!(unknown_08 <= other.unknown_08))
        unknown_08 = other.unknown_08;
    if (!(depth <= other.depth))
        depth = other.depth;
    float savedOffset = separationOffset;
    separationOffset = 0.0f;
    for (const SClsnContact *contact = other.contacts; contact; contact = contact->next)
        AddContact(*contact);
    separationOffset = savedOffset;
    return *this;
}

void CCollision::AddContact(const SClsnContact &contact) {
    SClsnContact *copy = ms_contactPool.Allocate();
    if (copy) {
        copy->separation = contact.separation + separationOffset;
        copy->normal = contact.normal;
        copy->position = contact.position;
        copy->feature = contact.feature;
        copy->subVolume = contact.subVolume;
        copy->next = contacts;
        contacts = copy;
        contactCount++;
    }
}

void CCollision::SwapOrder() {
    void *swap = first;
    first = second;
    second = swap;
}

extern float LARGEST_MOVE_PER_TICK;
// The original keeps this as a function-local static (isCSFWarningEnabled.6234).
extern int isCSFWarningEnabled asm("isCSFWarningEnabled.6234");

EClsnState CCollision::Test() {
    int kinds[3] = {0, 1, 2};
    int kind = kinds[unknown_10b];
    IVolume *firstVolume = (IVolume *)((ISceneNode *)first)->GetWorldBoundingVolume((ISceneNode::EVolumeType)kind);
    IVolume *secondVolume = (IVolume *)((ISceneNode *)second)->GetWorldBoundingVolume((ISceneNode::EVolumeType)kind);
    separationOffset = 0.0f;
    float firstRadius = firstVolume->GetRadius();
    firstVolume->SetRadius(firstRadius + LARGEST_MOVE_PER_TICK);
    float secondRadius = secondVolume->GetRadius();
    secondVolume->SetRadius(secondRadius + LARGEST_MOVE_PER_TICK);
    if (!(firstVolume->GetRadius() <= LARGEST_MOVE_PER_TICK)) {
        separationOffset += LARGEST_MOVE_PER_TICK;
        unknown_10c = 1;
    } else {
        unknown_10c = 0;
    }
    if (!(secondVolume->GetRadius() <= LARGEST_MOVE_PER_TICK)) {
        separationOffset += LARGEST_MOVE_PER_TICK;
        unknown_10d = 1;
    } else {
        unknown_10d = 0;
    }
    int state = 0;
    if (unknown_10a) {
        state = firstVolume->TestCollision(*secondVolume, *this, false);
    } else {
        int tested = firstVolume->TestCollision(*secondVolume, *this, true);
        if (tested > 0)
            state = tested;
        switch (tested) {
        case 1:
            lineTest = 0;
            break;
        case 2:
            if (lineTest)
                unknown_08 *= depth;
            break;
        }
        isCSFWarningEnabled = 1;
    }
    firstVolume->SetRadius(firstRadius);
    secondVolume->SetRadius(secondRadius);
    return (EClsnState)state;
}

void CCollision::ClearContacts() {
    while (contacts) {
        SClsnContact *next = contacts->next;
        ms_contactPool.Free(contacts);
        contacts = next;
        contactCount--;
    }
}
