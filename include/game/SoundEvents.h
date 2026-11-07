// AI-assisted scoped reconstruction from GR8E69; see docs/Sound.md.
#ifndef GAME_SOUND_EVENTS_H
#define GAME_SOUND_EVENTS_H
#pragma interface
#include "CVector3.h"

class Listener;

// Slot order follows the original tables. The two opaque pointer return types
// below are ABI views; their original pointee types remain unknown.
class SoundEmitter {
  public:
    virtual ~SoundEmitter() {}
    virtual void Clone(SoundEmitter *) const = 0;
    virtual float GetDistance() const = 0;
    virtual int GetAzimuth() const = 0;
    virtual void *GetObjRef() const = 0;
    virtual int GetType() = 0;
    virtual void *GetFilterDef() const;
};

class PositionSoundEmitter : public SoundEmitter {
  public:
    float x, y, z; // +4, +8, +12; no CVector3 fourth component.
    PositionSoundEmitter(); // The inline constructor remains research.
    virtual ~PositionSoundEmitter();
    virtual void Clone(SoundEmitter *) const;
    virtual float GetDistance() const;
    virtual int GetAzimuth() const;
    virtual void *GetObjRef() const;
    virtual int GetType();
    void SetPosition(const CVector3 &);
};

class SoundEvent {
  public:
    SoundEmitter *emitter; // +0
    Listener *listener;   // +4
    int field_08;
    float field_0c;
    int field_10[3];
    int field_1c;
    // The compiler places the table pointer at +0x20. Names of the unknown
    // fields describe offsets, not recovered parameter meanings.
    SoundEvent(); // No accepted default-constructor body yet.
    virtual ~SoundEvent() {}
    virtual int SendEvent() = 0;
    virtual int UpdateEvent(unsigned int);
    virtual int EndEvent(unsigned int);
    virtual void SetEmitter(SoundEmitter *);
    virtual void SetListener(Listener *);
};

class SoundAliasEvent : public SoundEvent {
  public:
    unsigned int alias; // +0x24; passed to cSoundMan::GetSoundEntries.
    SoundAliasEvent(); // No accepted default-constructor body yet.
    virtual ~SoundAliasEvent();
    virtual int SendEvent();
};

typedef char SoundEmitterSizeCheck[sizeof(SoundEmitter) == 4 ? 1 : -1];
typedef char PositionSoundEmitterSizeCheck[sizeof(PositionSoundEmitter) == 16 ? 1 : -1];
typedef char SoundEventSizeCheck[sizeof(SoundEvent) == 36 ? 1 : -1];
typedef char SoundAliasEventSizeCheck[sizeof(SoundAliasEvent) == 40 ? 1 : -1];
#endif
