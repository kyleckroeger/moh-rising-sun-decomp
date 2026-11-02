// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AnimObjFactory.cpp; member names are descriptive.
#include "SceneObjects.h"

void DebugMsg(const char *, ...);
// The factory's game-object interface sits at +0x3c in an animated object.
class IGameObject {
  public:
    void IGO_Destroy();
};
class CAnimObjectFactory {
  public:
    void ReleaseObject(CAnimObject *);
};

void CAnimObjectFactory::ReleaseObject(CAnimObject *object) {
    if (object) {
        DebugMsg("*** Animation Object Factory Release, addr: 0x%08x bsobject: %p\n", object, ((ISceneNode *)object)->GetScriptObject());
        ((CAnimObjectInterface *)object)->Shutdown();
        ((IGameObject *)((char *)object + 0x3c))->IGO_Destroy();
    }
}
