// AI-assisted reconstruction of the tasks derived from EALA::Character::Task; see docs/Animation.md.
// Class and method names come from the symbol table; member names are inferred from their use.
#ifndef MOH_CHARACTERTASKS_H
#define MOH_CHARACTERTASKS_H
#include "CharacterTask.h"
namespace EALA {
namespace Character {
// A task that redirects evaluation to another substate of its state.
class Jump : public Task {
  public:
    static Resource::Pointer<Jump> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                          unsigned int target, unsigned int blendFrames);
    Jump(Resource::Pointer<State> &state, unsigned int substate, Task *parent, unsigned int target,
         unsigned int blendFrames);
    virtual ~Jump();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual Jump *AsJump();
    void Assign(Resource::Pointer<Task> &task);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    unsigned int target;           // +0x34
    unsigned int blendFrames;      // +0x38
    int evaluated;                 // +0x3c
    Resource::Pointer<Task> kept;  // +0x40
};
// A task that sends one message to its parent, then holds a kept task or the still pose.
class Message : public Task {
  public:
    static Resource::Pointer<Message> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Message(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Message();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual Message *AsMessage();
    void Assign(Resource::Pointer<Task> &task);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    int sent;                      // +0x34
    float poseAge;                 // +0x38
    unsigned int unknown_3c;
    Resource::Pointer<Task> kept;  // +0x40
};
// Runs up to sixteen tasks in order and promotes the last one into its parent when it is alone.
class Sequence : public Task {
  public:
    static Resource::Pointer<Sequence> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Sequence(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Sequence();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual void Promote(Resource::Pointer<Task> &task);
    void AddTask(Resource::Pointer<Task> task);
    void Pop();
    void AttemptPromotion();
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    Resource::Pointer<Task> tasks[16]; // +0x34
    unsigned int count;                // +0x74
    Resource::Pointer<Task> last;      // +0x78; the task that was promoted.
};
// Blends from one task into another over a fixed time, then promotes the target into its parent.
class Transition : public Task {
  public:
    static Resource::Pointer<Task> Create(Task *parent, Resource::Pointer<Task> from, Resource::Pointer<Task> to,
                                          float time);
    Transition(Task *parent, Resource::Pointer<Task> from, Resource::Pointer<Task> to, float time);
    virtual ~Transition();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual void HandleMessage();
    virtual void HandlePose(unsigned int pose);
    virtual void HandleRotation(float rotation);
    virtual void HandleCycleEnd();
    virtual void HandleAnimDuration(float duration);
    virtual void Promote(Resource::Pointer<Task> &task);
    void PerformBlend(float weight, float time, float *pose);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    Resource::Pointer<Task> from; // +0x34
    Resource::Pointer<Task> to;   // +0x38
    float remaining;              // +0x3c
    float duration;               // +0x40
    int forwarding;               // +0x44; events pass to the parent only while set.
};
// The task a StateIterator installs when no substate settles: it holds the skeleton's still pose.
class Sentinel : public Task {
  public:
    static Resource::Pointer<Task> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Sentinel(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Sentinel();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
};
// One step of the task tree that re-evaluates its state's substate as children finish.
class StateIterator : public Task {
  public:
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    static Pointer<StateIterator> Alloc(Pointer<State> &state, unsigned int substate, Task *parent);
    StateIterator(Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~StateIterator();
    void Set(Pointer<State> &state, unsigned int substate, float time);
    void Set(unsigned int substate, float time);
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual void HandleMessage();
    virtual void HandleOpcode(Opcode &opcode);
    virtual void HandlePose(unsigned int pose);
    virtual void HandleRotation(float rotation);
    virtual void HandleCycleEnd();
    virtual void HandleAnimDuration(float duration);
    virtual void Promote(Pointer<Task> &task);
    virtual StateIterator *AsIterator();

  private:
    Pointer<Task> child; // +0x34
};
} // namespace Character
} // namespace EALA
#endif
