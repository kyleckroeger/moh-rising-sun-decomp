// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp. Each class embeds script data at +0x14
// after the BSGO_Basic prefix; the scene node and proximity offsets differ per class.
#include "BSGameObject.h"

class BSGO_Destructable : public BSGO_Basic {
  public:
    virtual ~BSGO_Destructable();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    unsigned char scriptData[0x34 - 0x14];
    ISceneNode *sceneNode; // +0x34
    unsigned char proximityData[4]; // +0x38; extent unknown.
};
void *BSGO_Destructable::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Destructable::GetSceneNode() { return sceneNode; }
void *BSGO_Destructable::GetProximityData() { return proximityData; }

class BSGO_Anim : public BSGO_Basic {
  public:
    virtual ~BSGO_Anim();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    unsigned char scriptData[0x2c - 0x14];
    ISceneNode *sceneNode; // +0x2c
    unsigned char proximityData[4];
};
void *BSGO_Anim::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Anim::GetSceneNode() { return sceneNode; }
void *BSGO_Anim::GetProximityData() { return proximityData; }

class BSGO_Soldier : public BSGO_Basic {
  public:
    virtual ~BSGO_Soldier();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    unsigned char scriptData[0xf8 - 0x14];
    ISceneNode *sceneNode; // +0xf8
    unsigned char proximityData[4]; // +0xfc; extent unknown.
};
void *BSGO_Soldier::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Soldier::GetSceneNode() { return sceneNode; }
void *BSGO_Soldier::GetProximityData() { return proximityData; }

class BSGO_Tank : public BSGO_Basic {
  public:
    virtual ~BSGO_Tank();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    unsigned char scriptData[0xf8 - 0x14];
    ISceneNode *sceneNode; // +0xf8
    unsigned char proximityData[4]; // +0xfc; extent unknown.
};
void *BSGO_Tank::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Tank::GetSceneNode() { return sceneNode; }
void *BSGO_Tank::GetProximityData() { return proximityData; }

class BSGO_Player : public BSGO_Basic {
  public:
    virtual ~BSGO_Player();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    unsigned char scriptData[0x38 - 0x14];
    ISceneNode *sceneNode; // +0x38
    unsigned char proximityData[4]; // +0x3c; extent unknown.
};
void *BSGO_Player::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Player::GetSceneNode() { return sceneNode; }
void *BSGO_Player::GetProximityData() { return proximityData; }
