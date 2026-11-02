// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "GameSettings.h"

void CHierObject::RegisterBulletDamage(int index, int minClass, int maxClass, float multiplier) {
    if (index > 0) {
        ((CHierObject *)GetSubObject(index))->RegisterBulletDamage(0, minClass, maxClass, multiplier);
        return;
    }
    for (int i = 0; i <= 1; i++) {
        int used = bulletDamage[i].minClass != -1;
        if (!used) {
            bulletDamage[i].minClass = minClass;
            bulletDamage[i].maxClass = maxClass;
            bulletDamage[i].multiplier = multiplier;
            return;
        }
    }
}

float CHierObject::GetBulletDamageMultiplierByClass(int index, int bulletClass) {
    if (index > 0)
        return ((CHierObject *)GetSubObject(index))->GetBulletDamageMultiplierByClass(0, bulletClass);
    for (int i = 0; i <= 1; i++) {
        if (bulletClass >= bulletDamage[i].minClass && bulletClass <= bulletDamage[i].maxClass)
            return bulletDamage[i].multiplier;
    }
    return 0.0f;
}

void CHierObject::SerializeSaveData() {
    CVector3 position;
    CVector3 *savedPosition = &position;
    CQuaternion rotation;
    ((ISceneNode *)this)->GetPosition(*savedPosition);
    GetRotation(rotation);
    g_GameSettings.SerializeValue(*savedPosition);
    g_GameSettings.SerializeValue(rotation);
    ((IMovingSceneNode *)this)->SetPosition(*savedPosition);
    SetRotation(rotation);
    if (g_GameSettings.SerializingIn())
        attachOffset = *savedPosition;
}

int CHierObject::GetObjectType() const { return 8; }
CHierObject *CHierObject::AsHierObject() { return this; }
const CHierObject *CHierObject::AsHierObject() const { return this; }
int CHierObject::SetAnimationState(const char *, int) { return 0; }
