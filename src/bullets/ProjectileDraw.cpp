// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void DebugMsg(const char *, ...);
enum ERenderBins { RenderBinUnknown = 0 };
// The command record a draw bin stores; only what Draw writes is named.
struct DrawCommand {
    int sortKey;
    int field_4;
    unsigned char unknown_8[4];
    void *owner;
    unsigned char unknown_10[0x1c - 0x10];
    void (*execute)(CDrawContext &, void **);
    CVector3 offset;
};
// A view's camera matrix at +0x50.
struct DrawView {
    unsigned char unknown_0[0x50];
    CMatrix matrix;
};
class CDrawContext {
  public:
    unsigned char unknown_0[212];
    DrawView *view;
    void AddCommand(ERenderBins, const DrawCommand &);
};
static inline void SetDiff(CVector3 &r, const CVector3 &a, const CVector3 &b) { r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z; }
void CProjectileBullet::Draw(CDrawContext &context) {
    if (properties->width == 0.0f) return;
    int shutDown = flags.shutDown;
    if (shutDown) return;
    if (!(life <= 0.0f) && !flags.hitPending) return;
    DrawCommand command;
    SetDiff(command.offset, localToWorld.GetPos(), context.view->matrix.GetPos());
    command.sortKey = (int)command.offset.LengthSquared();
    command.field_4 = shutDown;
    command.execute = ExecuteDraw;
    command.owner = this;
    context.AddCommand((ERenderBins)9, command);
}
