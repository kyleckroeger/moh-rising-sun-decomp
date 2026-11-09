// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
#include "SceneObjects.h"
extern int g_bInMultiplayerMode;
void CPlayerWeaponObject::BeginUpdate(float dt) {
    if (weapon && !g_bInMultiplayerMode) {
        int reserve = weapon->reserveAmmo;
        if (reserve != field_3f4) field_3f4 = reserve;
    }
    if (visible) {
        ((CAnimObjectInterface *)this)->UpdateAnimation(dt);
        if (!player->flags.hideWeapon) {
            UpdateWeaponTransforms();
            weapon->Update(dt, false);
            UpdateMuzzleFlash();
        }
    }
}
void CPlayerWeaponObject::AttemptUpdate(float) {}
void CPlayerWeaponObject::CommitUpdate() {}
void DebugMsg(const char *, ...);
enum ERenderBins { RenderBinUnknown = 0 };
// The command record a draw bin stores; only what Draw writes is named.
struct DrawCommand {
    int sortKey;
    int field_4;
    unsigned char unknown_8[4];
    void *owner;
    int field_10;
    int field_14;
    int field_18;
    void (*execute)(CDrawContext &, void **);
};
// The context's binding at +0x114; +0x18 identifies the view owner the player's +0x10d8 names.
struct DrawBinding {
    unsigned char unknown_0[0x18];
    int owner;
};
class CDrawContext {
  public:
    unsigned char unknown_0[192];
    CMatrix *transform;
    unsigned char unknown_c4[0x114 - 0xc4];
    DrawBinding *binding;
    void AddCommand(ERenderBins, const DrawCommand &);
};
void CPlayerWeaponObject::Draw(CDrawContext &context) {
    if (context.binding->owner != player->viewWord) return;
    if (state > 0) { state--; return; }
    if (!field_2d8) { DebugMsg("CPlayerWeaponObject::Draw(): animation for 0x%08x has not been updated.\n                             Skipping draw that would result in a frame of still pose.\n\n", this); return; }
    CMatrix *saved = context.transform;
    context.transform = &transform;
    DrawCommand command;
    command.field_10 = field_c8;
    command.field_4 = 0;
    command.owner = this;
    command.execute = DrawWeapon;
    command.field_14 = 1;
    command.field_18 = 0;
    command.sortKey = (int)this;
    context.AddCommand((ERenderBins)11, command);
    context.transform = saved;
}
