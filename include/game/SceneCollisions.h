// AI-assisted reconstruction; the header name is a project choice. The inline operations of CScene's
// collision vector and node pairs, which need the complete CCollision.
#ifndef GAME_SCENECOLLISIONS_H
#define GAME_SCENECOLLISIONS_H
#include "SceneNode.h"
#include "Volume.h"

inline CScene::SCollisionIter &CScene::SCollisionIter::operator++() {
    p++;
    return *this;
}
inline CScene::SCollisionConstIter &CScene::SCollisionConstIter::operator++() {
    p++;
    return *this;
}
inline CScene::SCollisionIter CScene::SCollisionVec::End() { return SCollisionIter(data + count); }
inline void CScene::SCollisionVec::Clear() {
    for (SCollisionIter it = Begin(); it != End(); ++it)
        it.p->~CCollision();
    count = 0;
}
inline void CScene::SCollisionVec::PushBack(const CCollision &collision) {
    SCollisionIter it = End();
    new (it.p) CCollision(collision);
    count++;
}
inline bool operator==(const CScene::UNodePair &a, const CScene::UNodePair &b) {
    return a.nodes.first == b.nodes.first && a.nodes.second == b.nodes.second;
}
inline bool operator>(const CScene::UNodePair &a, const CScene::UNodePair &b) {
    return a.nodes.first > b.nodes.first || (a.nodes.first == b.nodes.first && a.nodes.second > b.nodes.second);
}
#endif
