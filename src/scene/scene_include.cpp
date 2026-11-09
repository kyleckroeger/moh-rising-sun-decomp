// CScene::IncludeCollision drops a pair recorded by ExcludeCollision.
#include "SceneNode.h"

template <class T> static inline T *FindEntry(T *first, T *last, const T &value) {
    for (; first != last; ++first) {
        if (*first == value)
            break;
    }
    return first;
}

void CScene::IncludeCollision(const ISceneNode &a, const ISceneNode &b) {
    SExclusionEntry entry(&a, &b, 0.0f);
    SExclusionEntry *found = FindEntry(exclusions.Begin(), exclusions.End(), entry);
    if (found != exclusions.End())
        exclusions.Erase(found);
}
