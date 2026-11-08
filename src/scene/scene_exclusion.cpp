// CScene::SExclusionEntry keeps its node pair in address order.
#include "SceneNode.h"

CScene::SExclusionEntry::SExclusionEntry(const ISceneNode *a, const ISceneNode *b, float v) {
    if ((unsigned int)a < (unsigned int)b) {
        low = a;
        high = b;
    } else {
        low = b;
        high = a;
    }
    value = v;
}
