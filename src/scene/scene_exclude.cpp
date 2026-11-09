// CScene::ExcludeCollision records a node pair that must not collide. When a player is excluded from
// a moving object of collision id 23, the player's script object also receives event 40 with a
// bullet-hit record of maximum damage, zero if the player is invincible.
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"
#include "ScriptTimers.h"

void *BSEventGetEventMemoryBlock(unsigned int);

// The bullet-hit event's record, as sent by the explosion and bullet code.
struct BS_STRUCT_HitByBulletInfo_struct {
    BSObject *source;
    float damage;
    int direction;
    int bulletType;
    int location;
    int unknown_14[3];
};

static inline int MotionPlaying(const CHierObject *object) { return object->flags.motionPlaying; }

template <class T> static inline T *FindEntry(T *first, T *last, const T &value) {
    for (; first != last; ++first) {
        if (*first == value)
            break;
    }
    return first;
}

void CScene::ExcludeCollision(const ISceneNode &a, const ISceneNode &b, int value) {
    SExclusionEntry entry(&a, &b, (float)value);
    CPlayerObject *player = (CPlayerObject *)(a.AsPlayerObject() ? a.AsPlayerObject() : b.AsPlayerObject());
    if (player) {
        CHierObject *other = (CHierObject *)(b.AsHierObject() ? b.AsHierObject() : a.AsHierObject());
        if (other && ((ISceneNode *)other)->GetCollisionId() == 23) {
            bool moving = false;
            if (other->ScriptMoving() || other->ScriptRotating() || MotionPlaying(other))
                moving = true;
            if (moving || other->pathState > 1) {
                BS_STRUCT_HitByBulletInfo_struct *event = (BS_STRUCT_HitByBulletInfo_struct *)BSEventGetEventMemoryBlock(sizeof(BS_STRUCT_HitByBulletInfo_struct));
                event->bulletType = 0;
                event->damage = 2147483647.0f;
                event->source = 0;
                event->direction = 0;
                if (player->IsInvincible())
                    event->damage = 0.0f;
                BSObjectTriggerEvent((BSObject *)((ISceneNode *)player)->GetScriptObject(), 40, event, 0, false);
            }
        }
    }
    SExclusionEntry *found = FindEntry(exclusions.Begin(), exclusions.End(), entry);
    if (found == exclusions.End())
        exclusions.items[exclusions.count++] = entry;
}
