// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive. Draw queues up to
// two passes of the mesh, the second sorted by its squared distance from the camera. A
// multiplayer dump object also spins and doubles in size first.
#include "SceneObjects.h"
#include "DrawContext.h"
#include "StaticMesh.h"
#include "Light.h"
extern int g_bInMultiplayerMode;

// The command a draw bin stores; only what this file writes is named.
struct DrawCommand {
    int sortKey;
    int unknown_4;
    int unknown_8;
    void *owner;      // +0x0c
    void *part;       // +0x10
    int index;        // +0x14
    int unknown_18;
    void (*execute)(CDrawContext &, void **); // +0x1c
};

// A view's camera matrix at +0x50.
struct DrawView {
    unsigned char unknown_0[0x50];
    CMatrix matrix;
};
struct DrawContextView {
    unsigned char unknown00[212];
    DrawView *view;    // +0xd4
    unsigned char unknownd8[252 - 216];
    IMovingSceneNode *node; // +0xfc
};

static inline void RotateInPlace(CMatrix *m, const CVector3 &axis, float angle) {
    CMatrix rotation;
    CMatrix *r = &rotation;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    CVector3 pos = m->GetPos();
    m->SetPos(CVector3(0.0f, 0.0f, 0.0f));
    r->BuildRot(axis, angle);
    CMatrix::s_TempMat = *m;
    m->Multiply(*r, CMatrix::s_TempMat);
    m->SetPos(pos);
}

static inline CVector3 ZAxis() { return CVector3(0.0f, 0.0f, 1.0f); }

void CStaticObject::Draw(CDrawContext &context) {
    CStaticMesh *mesh = (CStaticMesh *)model;
    mesh->SetFrame(motionStart);
    ((CStaticMesh *)model)->RebindAnimatedTextures();
    CMatrix *saved = context.modelView;
    if (g_bInMultiplayerMode && flags.dumpObject) {
        float angle = dumpAge;
        float low = -3.1415927410125732f, high = 3.1415927410125732f;
        if (!(angle <= high)) {
            do {
                angle -= 6.2831854820251465f;
            } while (angle > high);
        }
        if (!(angle >= low)) {
            do {
                angle += 6.2831854820251465f;
            } while (angle < low);
        }
        dumpAge = angle;
        CVector3 position = transform.GetPos();
        CVector3 scale(2.0f, 2.0f, 2.0f);
        transform.BuildScale(scale);
        const CVector3 &axis = ZAxis();
        RotateInPlace(&transform, axis, -dumpAge);
        transform.SetPos(position);
    }
    context.modelView = &transform;
    if (((CStaticMesh *)model)->mesh->part0) {
        DrawCommand command;
        command.owner = this;
        command.execute = ExecuteDraw;
        command.part = ((CStaticMesh *)model)->mesh->part0;
        command.index = 0;
        context.AddCommand((ERenderBins)4, command);
    }
    if (((CStaticMesh *)model)->mesh->part1) {
        DrawCommand command;
        CVector3 position;
        ((CAnimObjectInterface *)this)->GetPosition(position);
        position -= ((DrawContextView &)context).view->matrix.GetPos();
        command.execute = ExecuteDraw;
        command.sortKey = (int)position.LengthSquared();
        command.owner = this;
        command.part = ((CStaticMesh *)model)->mesh->part1;
        command.index = 1;
        context.AddCommand((ERenderBins)9, command);
    }
    DrawDebugInfo(context);
    ((CStaticMesh *)model)->SetFrame(0);
    context.modelView = saved;
}

class CNormColor {
  public:
    float r, g, b, a;
};
class GlobalColorModifier {
  public:
    static void SetScale(const CNormColor &);
};

void CStaticObject::ExecuteDraw(CDrawContext &context, void **parameters) {
    CStaticObject *self = (CStaticObject *)parameters[0];
    int mode;
    if (((CStaticMesh *)self->model)->IsLightingEnabled() || self->fade.AreFading())
        mode = 4;
    else
        mode = 3;
    void *meshParameters[4];
    meshParameters[0] = self->model;
    meshParameters[1] = (void *)mode;
    meshParameters[2] = ((DrawContextView &)context).node;
    meshParameters[3] = parameters[2];
    CNormColor color;
    color.a = 1.0f;
    color.g = color.b = color.r = 1.0f;
    color.a = self->FadeValue();
    GlobalColorModifier::SetScale(color);
    CLight::SetLightBlock(self->lightVolumes.GetVolume(), &context, 1.0f);
    CStaticMesh::DrawMesh(context, meshParameters);
    color.a = 1.0f;
    GlobalColorModifier::SetScale(color);
}

void CStaticObject::DrawDebugInfo(CDrawContext &) {}
