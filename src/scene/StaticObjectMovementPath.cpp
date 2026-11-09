// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AIFilterGlobal.h"
#include "AISplinePath.h"

#define TRAVERSAL ((CAISplinePathTraversal *)pathTraversal)
#define LOOK_TRAVERSAL ((CAISplinePathTraversal *)lookTraversal)

extern CAIFilterGlobal g_aigAIFilterGlobalObject;

void CStaticObject::SetMovementPath(int path, bool reverse, bool follow, float maxSpeed, float acceleration, float f3,
                                    float f4, BSObject *, float followDistance, bool keepSpeed) {
    if (path > 0) {
        CAISplinePath *spline = g_aigAIFilterGlobalObject.GetSplinePath(path, reverse);
        if (spline == TRAVERSAL->path) {
            if (!TRAVERSAL->state || !pathState)
                return;
        } else {
            TRAVERSAL->PrepareForTraversalForward(spline, false, 0);
            if (follow) {
                LOOK_TRAVERSAL->PrepareForTraversalForward(spline, false, 0);
                LOOK_TRAVERSAL->state = 3;
            }
        }
        if (!keepSpeed)
            pathSpeed = 0.0f;
        pathMaxSpeed = pathField104 = maxSpeed;
        pathAcceleration = acceleration;
        pathPitchRate = pathYawRate = 0.0f;
        pathMaxTurnRate = f3;
        pathTurnAcceleration = f4;
        if (follow) {
            pathState = 3;
            pathFollowTarget = g_scene.GetPlayer(0);
            pathFollowDistanceSquared = followDistance * followDistance;
        } else {
            pathState = 2;
        }
    } else if (path == -1) {
        if (!pathState)
            TRAVERSAL->state = 0;
        else
            pathState = 6;
    } else if (path == -2) {
        pathSpeed = 0.0f;
        pathState = 0;
        TRAVERSAL->state = 0;
        pathMaxSpeed = 0.0f;
    } else {
        if (!pathState)
            TRAVERSAL->state = 0;
        else
            pathState = 5;
    }
}
