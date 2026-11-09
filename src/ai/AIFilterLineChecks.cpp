// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AIFilter.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "AIFilterGlobal.h"
#include "Volume.h"
#include "CLine3.h"
#include "Fiber.h"
#include "AITargeting.h"
#include "BSGameObject.h"

// The destructor is implicit: the image's weak copy stores only the base table. Each checker
// sits in the scope of its own arm, so the arms share one frame slot and only jump optimization
// merges their tails.
class MechanicCollisionChecker : public dwi::IVisitor<ISceneNode> {
  public:
    MechanicCollisionChecker(const CVolFiber &f, float d, ISceneNode *a, ISceneNode *b, int stop, int skipScript)
        : fiber(f), stopAtFirstHit(stop), ignoreScript(skipScript), distance(d), nearest(d), skipA(a), skipB(b) {}
    virtual bool Visit(ISceneNode &);
    unsigned char unknown04[12];
    CVolFiber fiber;       // +0x10
    int stopAtFirstHit;    // +0x60
    int ignoreScript;      // +0x64
    float distance;        // +0x68
    float nearest;         // +0x6c
    ISceneNode *skipA;     // +0x70
    ISceneNode *skipB;     // +0x74
    // Inferred: all three line checks reserve 128 bytes for a checker; nothing here reads the rest.
    unsigned char unknown78[8];
};

int CAIFilterGlobal::CheckVisionLineToEnvironment(const CVector3 &start, const CVector3 &end, ISceneNode *skipA, ISceneNode *skipB) {
    CLine3 line;
    line.Set(start, end);
    CScene *scene = &g_scene;
    ISceneNode *world = (ISceneNode *)scene->world;
    CCollision collision;
    int result = 1;
    CVolFiber fiber(line);
    IVolume *volume = (IVolume *)world->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
    if (volume) {
        collision.lineTest = 0;
        result = volume->TestCollision(line, collision, false);
    }
    int hit = result != 1;
    if (!hit) {
        if (skipB) {
            MechanicCollisionChecker checker(fiber, -1.0f, skipA, skipB, 1, 1);
            hit = scene->VisitAllStaticObjects(checker);
        } else {
            MechanicCollisionChecker checker(fiber, -1.0f, skipA, 0, 1, 1);
            hit = scene->VisitAllStaticObjects(checker);
        }
    }
    return hit;
}

int CAIFilterGlobal::CheckCollisionLineToEnvironment(const CVector3 &start, const CVector3 &end, ISceneNode *skipA, ISceneNode *skipB) {
    CLine3 line;
    line.Set(start, end);
    CScene *scene = &g_scene;
    ISceneNode *world = (ISceneNode *)scene->world;
    CCollision collision;
    int result = 1;
    CVolFiber fiber(line);
    IVolume *volume = (IVolume *)world->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
    if (volume) {
        result = volume->TestCollision(fiber, collision, false);
    }
    int hit = result != 1;
    if (!hit) {
        if (skipB) {
            MechanicCollisionChecker checker(fiber, -1.0f, skipA, skipB, 1, 0);
            hit = scene->VisitAllStaticObjects(checker);
        } else {
            MechanicCollisionChecker checker(fiber, -1.0f, skipA, 0, 1, 0);
            hit = scene->VisitAllStaticObjects(checker);
        }
    }
    return hit;
}

float CAIFilterGlobal::GetCollisionDistLineToEnvironment(const CVector3 &start, const CVector3 &end, CAIFilterObject *filter, float distance) {
    CLine3 line;
    line.Set(start, end);
    CCollision collision;
    CScene *scene = &g_scene;
    ISceneNode *world = (ISceneNode *)scene->world;
    int result = 1;
    if (!world) {
        return -1.0f;
    }
    CVolFiber fiber(line);
    IVolume *volume = (IVolume *)world->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
    if (volume) {
        result = volume->TestCollision(fiber, collision, true);
    }
    int hit = result != 1;
    if (hit) {
        distance *= collision.depth;
    }
    ISceneNode *skip = 0;
    if (filter) {
        skip = (*(BSGO_Basic **)((char *)filter->scriptObject + 0x20))->GetSceneNode();
    }
    MechanicCollisionChecker checker(fiber, distance, skip, 0, 0, 0);
    scene->VisitAllStaticObjects(checker);
    if (!(checker.nearest >= distance)) {
        distance = checker.nearest;
        hit = 1;
    }
    if (hit) {
        return distance;
    }
    return -1.0f;
}
