// AI-assisted reconstruction of the character animation task tree; see docs/Animation.md.
// Class and method names come from the symbol table; member names are inferred from their use.
#ifndef MOH_CHARACTERTASK_H
#define MOH_CHARACTERTASK_H
#include "ResourceTypes.h"
#include "TargetTypes.h"
#include "EAGLTransform.h"
void DebugMsg(const char *, ...);
namespace EALA {
namespace Character {
class Task : public Resource, public ISubject {
  public:
    Task(State *state, unsigned int substate, Task *parent);
    virtual ~Task();
    virtual int Busy();
    virtual void Evaluate(float time, float *pose);
    virtual void EvalPose(float time, float *pose);
    virtual float GetAnimDuration();
    virtual COORD3 GetAnimTranslation();
    virtual float GetTotalRootZAxisRotation();
    virtual void Dump(unsigned int depth) = 0;
    virtual void HandleMessage();
    virtual void HandleOpcode(Opcode &opcode);
    virtual void HandlePose(unsigned int pose);
    virtual void HandleRotation(float rotation);
    virtual void HandleCycleEnd();
    virtual void HandleAnimDuration(float duration);
    virtual void Promote(Pointer<Task> &task);
    virtual class StateIterator *AsIterator();
    virtual class Message *AsMessage();
    virtual class Jump *AsJump();

    State *GetState() const;
    void Reparent(Task *parent);
    void SetState(State *state);
    void SetSubstate(unsigned int substate);
    void SetCorePose(unsigned int pose);
    unsigned int GetCorePose() const;
    StateIterator *ParentIterator();
    void DumpPrefix(unsigned int depth);

    Pointer<State> state;        // +0x10
    unsigned int substate;       // +0x14
    WeakPtr<Task, 8> parent;     // +0x18
    unsigned int corePose;       // +0x30
};
class State {
  public:
    Resource::Pointer<Task> EvalSubstate(unsigned int substate, Task *parent);
    Resource::Pointer<Task> FindAuthoredTransition(Task *task, unsigned int fromPose, unsigned int toPose);
};
} // namespace Character
} // namespace EALA
#endif
