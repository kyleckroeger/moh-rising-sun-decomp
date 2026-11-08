// AI-assisted reconstruction from GR8E69. Part of the original anim_obj.cpp; member names are
// descriptive. StateDone is the task handed to the choreographer when a state is set; the class
// name comes from its symbols, and only what these two functions use is declared.
#include "SceneObjects.h"
#include "ScriptTimers.h"

namespace EALA {
class Task {
  public:
    virtual void Execute() = 0;
};
}

class CAnimObject::StateDone : public EALA::Task {
  public:
    StateDone();
    virtual void Execute();
    int event;
    CAnimObject *owner;
};

CAnimObject::StateDone::StateDone() : event(-1), owner(0) {}

void CAnimObject::StateDone::Execute() {
    if (owner)
        owner->stateDone = 1;
    if (event >= 0)
        BSObjectTriggerEvent((BSObject *)((CAnimObjectInterface *)owner)->GetScriptObject(), event, 0, 0, true);
}
