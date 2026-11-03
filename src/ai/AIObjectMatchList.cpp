// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

CBehavior *FindMatchingBehaviorFromList(CBehavior **list, int count, EBehaviorType type) {
    CBehavior *found = 0;
    for (int i = 0; i < count; i++) {
        if (list[i]->GetType() == type) {
            found = list[i];
            break;
        }
    }
    return found;
}
