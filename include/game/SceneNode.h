// AI-assisted scoped reconstruction; see docs/SceneNode.md.
#ifndef MOH_SCENENODE_H
#define MOH_SCENENODE_H
#pragma interface
#include "ObserverTypes.h"
#include "DwiContainers.h"

class CBullet;
class CAIDoodad;
class CAIObject;
class CAnimObject;
class CAnimatedPlayer;
class CBotObject;
class CCamera;
class CCollisionVolume;
class CSkyBox;
class CHierObject;
class CPlayerObject;
class CPlayerWeaponObject;
class CProjector;
class CSoldierObject;
class CStaticObject;
class CWorldObject;
class CCollision;
class CDrawContext;
class CFog;
class CLight;
class CMatrix;
class CVector3;
class BPDLightVolume;
class IMovingSceneNode;
// Parameter enum; its members are unknown.
enum EClsnId { ClsnIdUnknown = 0 };

// Virtual slots 5-67, in the order of the original ISceneNode table at 0x802e1680.
// Names and parameter types come from original symbols. Return types are not
// encoded in symbols: pointer returns follow the casts' and accessors' overrides,
// and int returns are ABI placeholders; see docs/SceneNode.md. Data members are
// not declared; complete size and layout remain unknown.
class ISceneNode : public IObserver {
  public:
    enum EVolumeType { VolumeTypeUnknown = 0 };
    // One end of a node's extent along a sweep axis.
    struct SExtent {
        ISceneNode *node;
        int isBegin;
        float position;
        int unknown0c;
    };
    virtual ~ISceneNode();
    virtual void BeginUpdate(float);
    virtual void UpdateAI(float);
    virtual void CommitAI();
    virtual void AttemptUpdate(float);
    virtual void UpdateCollisionVolumes();
    virtual void OnCollision(const CCollision &);
    virtual int Constrain(float);
    virtual void CommitUpdate();
    virtual void PostUpdate(float);
    virtual void Draw(CDrawContext &);
    virtual int Attach(ISceneNode &, CMatrix *, unsigned int);
    virtual int Detach();
    virtual int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    virtual void GetTMLocalToWorld(CMatrix &) const;
    virtual CVector3 GetWorldLinearVelocity() const;
    virtual void GetPosition(CVector3 &) const;
    virtual void GetCentroid(CVector3 &) const;
    virtual void GetRightward(CVector3 &) const;
    virtual void GetForward(CVector3 &) const;
    virtual void GetUpward(CVector3 &) const;
    virtual int IsVisible(CDrawContext &) const;
    virtual unsigned int IsDrawEnabled() const;
    virtual void SetDrawEnabled(bool);
    virtual EClsnId GetCollisionId() const;
    virtual void SetCollisionId(EClsnId);
    virtual int GetScriptObject() const;
    virtual void TriggerScriptEvent(int, void *, bool);
    virtual void HandleBulletCollision(CBullet *, const CCollision &);
    virtual IMovingSceneNode *AsMovingNode();
    virtual const IMovingSceneNode *AsMovingNode() const;
    virtual CStaticObject *AsStaticObject();
    virtual const CStaticObject *AsStaticObject() const;
    virtual CHierObject *AsHierObject();
    virtual const CHierObject *AsHierObject() const;
    virtual CWorldObject *AsWorldObject();
    virtual const CWorldObject *AsWorldObject() const;
    virtual CAnimObject *AsAnimObject();
    virtual const CAnimObject *AsAnimObject() const;
    virtual CSoldierObject *AsSoldierObject();
    virtual const CSoldierObject *AsSoldierObject() const;
    virtual CPlayerObject *AsPlayerObject();
    virtual const CPlayerObject *AsPlayerObject() const;
    virtual CAnimatedPlayer *AsAnimatedPlayerObject();
    virtual const CAnimatedPlayer *AsAnimatedPlayerObject() const;
    virtual CPlayerWeaponObject *AsPlayerWeaponObject();
    virtual const CPlayerWeaponObject *AsPlayerWeaponObject() const;
    virtual CLight *AsLight();
    virtual const CLight *AsLight() const;
    virtual CProjector *AsProjector();
    virtual const CProjector *AsProjector() const;
    virtual CBullet *AsBullet();
    virtual const CBullet *AsBullet() const;
    virtual CCollisionVolume *AsCollisionVolume();
    virtual const CCollisionVolume *AsCollisionVolume() const;
    virtual CBotObject *AsBotObject();
    virtual const CBotObject *AsBotObject() const;
    virtual CAIDoodad *GetAIDoodad();
    virtual const CAIDoodad *GetAIDoodad() const;
    virtual CAIObject *GetAIObject();
    virtual void SetAttachedLight(CLight *, const CVector3 &);
    virtual CLight *GetAttachedLight() const;
    virtual void SerializeSaveData();
};

// Virtual slots 68-88, introduced after ISceneNode. No IMovingSceneNode table is
// emitted; the order comes from derived tables such as IParticleSystem's.
class IMovingSceneNode : public ISceneNode {
  public:
    IMovingSceneNode *AsMovingNode();
    const IMovingSceneNode *AsMovingNode() const;
    virtual void Reset();
    virtual void Halt();
    virtual void ApplyForceTo(const CVector3 &, float);
    virtual void SetTMLocalToWorld(const CMatrix &);
    virtual void PreTransform(const CMatrix &);
    virtual void Transform(const CMatrix &);
    virtual void Move(const CVector3 &);
    virtual void Rotate(const CVector3 &, float);
    virtual void Pitch(float);
    virtual void Roll(float);
    virtual void Yaw(float);
    virtual void SetPosition(const CVector3 &);
    virtual void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    virtual void Orthonormalize();
    virtual void EnterLightVolume(BPDLightVolume *);
    virtual void ExitLightVolume(BPDLightVolume *);
    virtual BPDLightVolume *GetLightVolume();
    virtual void SetLightVolumeTransitionDuration(float);
    virtual const char *GetName();
    virtual int GetTeam();
    virtual unsigned char GetPlayerIndex();
};

namespace dwi {
template <class T> class IVisitor {
  public:
    virtual ~IVisitor() {}
    virtual bool Visit(T &) = 0;
};
}
// The instantiation (and its table) belongs elsewhere in the original image.
extern template class dwi::IVisitor<ISceneNode>;
class IVolume;
// Member-only prefix of the scene container; visitor routines establish the list
// heads below. This does not establish the complete CScene allocation.
class CScene {
  public:
    ISceneNode *firstNode; // +0x00
    ISceneNode *lastNode;  // +0x04
    CPlayerObject *players[4]; // +0x08
    CWorldObject *world; // +0x18
    CCamera *camera;     // +0x1c
    CSkyBox *skyBox;     // +0x20
    ISceneNode *firstStaticObject; // +0x24
    ISceneNode *lastStaticObject;  // +0x28
    ISceneNode *firstAnimObject;   // +0x2c
    ISceneNode *lastAnimObject;    // +0x30
    ISceneNode *firstLight;        // +0x34
    ISceneNode *lastLight;         // +0x38
    ISceneNode *firstProjector;    // +0x3c
    ISceneNode *lastProjector;     // +0x40
    int nodeCount;                 // +0x44
    int staticObjectCount;         // +0x48
    int animObjectCount;           // +0x4c
    int lightCount;                // +0x50
    int projectorCount;            // +0x54
    int locked; // +0x58: changes queue while set; see Lock and Unlock.
    int playerCount; // +0x5c
    // +0x60, +0x78, +0x90: the visible lights, an unused list and the visible projectors, filled by Draw.
    dwi::fast_vec<ISceneNode *> fast60, fast78, fast90;
    // +0xa8, +0xd4: the two sweep lists.
    dwi::list<ISceneNode::SExtent> extents0, extents1;
    // A node pair met by a sweep, kept in address order. Only the fields used here are known.
    union UNodePair {
        struct {
            ISceneNode *first;
            ISceneNode *second;
            signed char order;
        } nodes;
        unsigned char data[16];
    };
    // +0x100, +0x118: the pairs found by the two sweeps.
    dwi::fast_vec<UNodePair> pairs0, pairs1;
    // +0x130: the live collisions. The iterator clears its pointer on destruction; the inline
    // operations that need the complete CCollision are in SceneCollisions.h.
    struct SCollisionIter {
        SCollisionIter(CCollision *q) : p(q) {}
        SCollisionIter(const SCollisionIter &o) : p(o.p) {}
        ~SCollisionIter() { p = 0; }
        bool operator!=(const SCollisionIter &o) const { return p != o.p; }
        SCollisionIter &operator++();
        CCollision *p;
    };
    struct SCollisionConstIter {
        SCollisionConstIter(const SCollisionIter &o) : p(o.p) {}
        SCollisionConstIter(const SCollisionConstIter &o) : p(o.p) {}
        ~SCollisionConstIter() { p = 0; }
        bool operator!=(const SCollisionIter &o) const { return p != o.p; }
        SCollisionConstIter &operator++();
        const CCollision *p;
    };
    struct SCollisionVec {
        SCollisionIter Begin() { return SCollisionIter(data); }
        SCollisionIter End();
        void Clear();
        void PushBack(const CCollision &);
        int Size() const { return count; }
        int count;
        int capacity;
        CCollision *data;
        int owned;
    } collisions;
    // An excluded node pair, kept in address order, and a value.
    struct SExclusionEntry {
        SExclusionEntry(const ISceneNode *, const ISceneNode *, float);
        const ISceneNode *low;
        const ISceneNode *high;
        float value;
        SExclusionEntry &operator=(const SExclusionEntry &other) {
            low = other.low;
            high = other.high;
            value = other.value;
            return *this;
        }
        bool operator==(const SExclusionEntry &other) const { return low == other.low && high == other.high; }
    };
    // +0x140: the collision exclusions; the capacity word is unread here and its name is inferred.
    struct SExclusionList {
        SExclusionEntry *items;
        int capacity;
        int count;
        SExclusionEntry *Begin() { return items; }
        SExclusionEntry *End() { return items + count; }
        void Clear() { count = 0; }
        void Erase(SExclusionEntry *entry) {
            SExclusionEntry *end = End();
            SExclusionEntry *to = entry;
            for (SExclusionEntry *from = entry + 1; from != end;)
                *to++ = *from++;
            count--;
        }
    } exclusions;
    unsigned char unknown14c[0x158 - 0x14c];
    // A deferred scene change: the handler to run and its two arguments.
    struct SModCommand {
        void (CScene::*function)(SModCommand &);
        void *arg0;
        void *arg1;
    };
    // +0x158: the capacity word is unread here and its name is inferred.
    struct SCommandQueue {
        SModCommand *items;
        int capacity;
        int count;
        SModCommand *End() { return items + count; }
        void Clear() { count = 0; }
    } queue;
    void AddPlayer(CPlayerObject *);
    void SetWorld(CWorldObject *);
    void SetCamera(CCamera *);
    void SetSkyBox(CSkyBox *);
    void AddPlayer_now(SModCommand &);
    void SetWorld_now(SModCommand &);
    void SetCamera_now(SModCommand &);
    void SetSkyBox_now(SModCommand &);
    void AddNode_now(SModCommand &);
    void RemoveNode_now(SModCommand &);
    void AddNodeExtents(ISceneNode &);
    void RemoveNodeExtents(ISceneNode &);
    void Update(float);
    void Draw(CDrawContext &, CFog &);
    void SweepAllExtents();
    int DetectSlowCollisions(float);
    int DetectFastCollisions(float);
    // Node list iterator returned by value.
    class NodeIterator {
      public:
        NodeIterator(ISceneNode *n) : node(n) {}
        NodeIterator(const NodeIterator &other) : node(other.node) {}
        ISceneNode *node;
    };
    NodeIterator GetNodesBegin() const;
    NodeIterator GetNodesEnd() const;
    void Lock();
    void Unlock();
    void Reset();
    void Shutdown();
    void Add(ISceneNode &, ISceneNode *);
    void Remove(ISceneNode &);
    int IsNodeInScene(const ISceneNode *) const;
    CPlayerObject *GetPlayer(int) const;
    CPlayerObject *FindClosestPlayer(const CVector3 &);
    void ExcludeCollision(const ISceneNode &, const ISceneNode &, int);
    void ReOrder(ISceneNode *, ISceneNode *);
    void IncludeCollision(const ISceneNode &, const ISceneNode &);
    bool VisitAllStaticObjects(dwi::IVisitor<ISceneNode> &);
    bool VisitAllLights(dwi::IVisitor<ISceneNode> &);
    bool VisitAllNodesInVolume(dwi::IVisitor<ISceneNode> &, const IVolume &, ISceneNode::EVolumeType, bool);
};
extern CScene g_scene;

#endif
