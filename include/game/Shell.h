// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SHELL_H
#define GAME_SHELL_H

struct KEYMAP;

// An input event record; +4 and +8 are cleared together by Reset.
struct CInputEvent {
    int field00;
    int field04;
    int value;
};

// Method-only view of the device manager at the start of the input manager.
class CDeviceManager {
  public:
    void SetPort(unsigned char, signed char);
    int GetNumDevices() const;
    int GetDeviceState(unsigned char) const;
    void SetActuatorState(unsigned char, unsigned char, unsigned char, float, float);
    CInputEvent *GetEventTable(unsigned char);
    const CInputEvent *GetEventTable(unsigned char) const;
    int IsActive(signed char) const;
    int GetPortNum(unsigned char) const;
    int GetCurrentState(unsigned char);

    unsigned char unknown0[9152];
};

// The input manager singleton; members are named by what these methods do with them.
class CInputManager : public CDeviceManager {
  public:
    void Init();
    void Shutdown();
    void SetController(unsigned char, signed char);
    void SetKeymap(unsigned char, KEYMAP *);
    int GetValue(unsigned char, unsigned short) const;
    int GetInputAnyDevice() const;
    bool IsReady(signed char) const;
    int IsActive(signed char) const;
    void SetFeedback(unsigned char, unsigned char, unsigned char, float, float);
    void Reset();
    void SetBlockVibrations(bool, unsigned char);
    void SetMasterBlockVibrations(bool, unsigned char);
    void SetSPLockedPort(unsigned char);
    int FindDeviceIDFromPortNum(unsigned char) const;
    void SetBadControllerPort(unsigned char);

    KEYMAP *keymap;
    KEYMAP *portKeymaps[4];
    int blockVibrations[4];
    int masterBlockVibrations[4];
    unsigned char unknown23f4[8];
    signed char controllers[16];
    int badControllerSet;
    unsigned char badControllerPort;
};
extern CInputManager g_inputMgr;

#endif
