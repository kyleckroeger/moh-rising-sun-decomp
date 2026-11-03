// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"
#include "SceneNode.h"
#include <stdio.h>

extern CBehavior *FindMatchingBehaviorFromList(CBehavior **, int, EBehaviorType);
extern void DebugMsg(const char *, ...);

CBehavior *FindMatchingBehaviorFromListByCRC(CBehavior **list, int count, int crc) {
    CBehavior *found = 0;
    for (int i = 0; i < count; i++) {
        if (list[i]->GetCRC() == crc) {
            found = list[i];
            break;
        }
    }
    return found;
}

CBehavior *CAIObject::FindMatchingBehviorFromCurrentState(EBehaviorType type) {
    int count;
    CBehavior **list;
    switch (highLevelState) {
    case 1: count = behaviorLists[0].count; list = behaviorLists[0].behaviors; break;
    case 2: count = behaviorLists[1].count; list = behaviorLists[1].behaviors; break;
    case 3: count = behaviorLists[2].count; list = behaviorLists[2].behaviors; break;
    case 4: count = behaviorLists[3].count; list = behaviorLists[3].behaviors; break;
    default: return 0;
    }
    return FindMatchingBehaviorFromList(list, count, type);
}

CBehavior *CAIObject::FindMatchingBehavior(EBehaviorType type) {
    CBehavior *found = FindMatchingBehviorFromCurrentState(type);
    if (found) return found;
    found = FindMatchingBehaviorFromList(behaviorLists[0].behaviors, behaviorLists[0].count, type);
    if (found) return found;
    found = FindMatchingBehaviorFromList(behaviorLists[1].behaviors, behaviorLists[1].count, type);
    if (found) return found;
    found = FindMatchingBehaviorFromList(behaviorLists[2].behaviors, behaviorLists[2].count, type);
    if (found) return found;
    return FindMatchingBehaviorFromList(behaviorLists[3].behaviors, behaviorLists[3].count, type);
}

CBehavior *CAIObject::FindMatchingBehaviorByCRC(int crc) {
    CBehavior *found = FindMatchingBehaviorFromListByCRC(behaviorLists[0].behaviors, behaviorLists[0].count, crc);
    if (found) return found;
    found = FindMatchingBehaviorFromListByCRC(behaviorLists[1].behaviors, behaviorLists[1].count, crc);
    if (found) return found;
    found = FindMatchingBehaviorFromListByCRC(behaviorLists[2].behaviors, behaviorLists[2].count, crc);
    if (found) return found;
    return FindMatchingBehaviorFromListByCRC(behaviorLists[3].behaviors, behaviorLists[3].count, crc);
}

void CAIObject::EnableBehavior(EBehaviorType type) {
    CBehavior *behavior = FindMatchingBehavior(type);
    if (behavior)
        behavior->Enable();
    else {
        char message[120];
        sprintf(message, "AIObject::EnableBehavior - no\tbehavior matches id\t%d\n", type);
        DebugMsg(message);
    }
}

void CAIObject::DisableBehavior(EBehaviorType type) {
    CBehavior *behavior = FindMatchingBehavior(type);
    if (behavior)
        behavior->Disable();
    else {
        char message[120];
        sprintf(message, "AIObject::DisableBehavior - no behavior matches id %d\n", type);
        DebugMsg(message);
    }
}

int CAIObject::IsBehaviorEnabled(EBehaviorType type) {
    CBehavior *behavior = FindMatchingBehavior(type);
    if (behavior)
        return behavior->enabled;
    char message[120];
    sprintf(message, "AIObject::DisableBehavior - no behavior matches id %d\n", type);
    DebugMsg(message);
    return 0;
}
