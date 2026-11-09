// AI-assisted reconstruction from GR8E69. Views of the A* search and area path node
// classes; class and method names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#ifndef GAME_AI_PATH_SEARCH_H
#define GAME_AI_PATH_SEARCH_H
#include "AITargeting.h"
#include "LinkedList.h"
class CAreaSearchNode : public CLinkedListElement {
  public:
    static void DeleteNode(CAreaSearchNode *);
    float GetCostValue(const CAreaSearchNode *);
    unsigned char unknown04[0x24 - 0x4];
    float bestCost; // +0x24; compared by GetBestPath.
};
class CAreaSearchNodeQueue {
  public:
    void Clear();
};
class CAreaSearchNodeList : public CLinkedList {};
// The search view of CAIAreaPathFinding: member offsets are used by the functions listed here.
class CAIAreaPathSearch {
  public:
    void SetupAStarPathWalkCommit();
    float PathDistance() const;
    void ClearSearchNodeQueue(CAreaSearchNodeQueue &);
    void ClearSearchNodeList(CAreaSearchNodeList &);
    void CleanupAStarPathWalk();
    void StopAStarPathWalk();
};
#endif
