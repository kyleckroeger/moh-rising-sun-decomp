// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
#ifndef GAME_CDBOBJECT_H
#define GAME_CDBOBJECT_H
#include "Volume.h"
#include "DwiList.h"

class CVolSphere;

struct CDBVector {
    float x, y, z;
};

// A triangle: vertex pointers, then surface and flag words read for the pass-through test.
struct CDBTri {
    const CDBVector *vertex[3];
    short flags;
    unsigned short surface;
};

class CDB;

class CSurfaceType {
  public:
    static void InitClass(const CDB *, const CDB *);
    static unsigned int GetDecalCRC(unsigned short, bool);
    static unsigned int GetParticleCRC(unsigned short, bool);
    static int GetSoundIndex(unsigned short, bool);
    static bool IsShootThru(unsigned short, bool);
    static bool IsPassThru(unsigned short, bool);
    static bool IsSeeThru(unsigned short, bool);
    static bool IsClimbable(unsigned short, bool);
};

// A plane (a three-float normal and a distance); the mark at +16 is negated once a group
// has been gathered; the triangle pointers follow from +20.
struct CDBTriGroup;

// Half sizes and four three-float rows, as the CCDBVolBox constructor reads them.
struct CDBBox {
    CDBVector halfSize;
    CDBVector row[4];
};

// A node's type is 1 for a branch.
struct CDBNode {
    int type;
};

struct CDBLeafNode : CDBNode {
    CDBBox box;
    int groupCount;
    CDBTriGroup *groups[1];
};

struct CDBBranchNode : CDBNode {
    const CDBBranchNode *front;
    const CDBBranchNode *back;
    CDBVector normal;
    float distance;
};

struct CDBTriGroup {
    CDBVector normal;
    float distance;
    int mark;
    CDBTri *tris[1];
};

// The collision database: the fields here are read by the CCDBObject tests. Their
// names are descriptive.
class CDB {
  public:
    unsigned char unknown_00[28];
    int branchCount;
    int surfaceTypeCount;
    unsigned char unknown_24[8];
    CDBLeafNode *leafRoot;
    CDBBranchNode *branchRoot;
    const void *surfaceTypes;
};

template <class T>
EClsnState TestCollisionWithTriGroups(const dwi::fast_vec<CDBTriGroup *> &, const T &, CCollision &, bool);

class CCDBObject : public IVolume {
  public:
    CCDBObject(CDB *);
    static void DisablePassThru(bool);
    void Init(CDB *);
    virtual ~CCDBObject();
    virtual EClsnState TestCollision(const IVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolBox &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolSphere &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolCapsule &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CCDBObject &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolFiber &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CAnimatedVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CWorldVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVector3 &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CLine3 &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CPlane &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CTriangle &, CCollision &, bool) const;
    virtual void GetExtents(CVector3 &, CVector3 &) const;
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    EClsnState TestCDBLeafNodeCollision(const CDBLeafNode &, const CVolBox &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBLeafNodeCollision(const CDBLeafNode &, const CVolSphere &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBLeafNodeCollision(const CDBLeafNode &, const CVolCapsule &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBLeafNodeCollision(const CDBLeafNode &, const CAnimatedVolume &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBBranchNodeCollision(const CDBBranchNode &, const CVolBox &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBBranchNodeCollision(const CDBBranchNode &, const CVolSphere &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBBranchNodeCollision(const CDBBranchNode &, const CVolCapsule &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    EClsnState TestCDBBranchNodeCollision(const CDBBranchNode &, const CAnimatedVolume &, CCollision &, bool, dwi::fast_vec<CDBTriGroup *> &) const;
    CDB *database;
    unsigned char unknown_08[8];
    CVector3 extentLow;
    CVector3 extentHigh;
};

#endif
