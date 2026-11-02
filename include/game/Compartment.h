// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. Only the members
// these fragments read are named; the offsets come from the code and the names are
// descriptive.
#ifndef GAME_COMPARTMENT_H
#define GAME_COMPARTMENT_H
#include "CDBObject.h"
#include "SceneNode.h"

class CDrawContext;
class CPTHeader;
class CRtcFile;

class CCompartment : public ISceneNode {
  public:
    virtual ~CCompartment();
    void Init(int, char *, CPTHeader *, CDB *, CRtcFile *);
    void Init(CRtcFile *);
    void InitCDB(int, char *, CDB *);
    static void GlobalReset();
    void DrawShadows(CDrawContext &);
    virtual unsigned int IsDrawEnabled() const;
    virtual int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    unsigned char unknown_after_node[64 - sizeof(ISceneNode)];
    CCDBObject collisionVolume;
    CCDBObject detailVolume;
    int index;
    char *name;
    CPTHeader *primerTable;
    CDB *database;
    unsigned char unknown_b0[4];
    unsigned int drawEnabled;
};

#endif
