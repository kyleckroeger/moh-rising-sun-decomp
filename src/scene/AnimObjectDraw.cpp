// AI-assisted reconstruction from GR8E69. Part of the original anim_obj.cpp; member names are
// descriptive. Draw queues the mesh's first pass, then, unless the mesh sets noSortedPass, a
// second pass sorted by the object's squared view depth. ClipSphere tests a bounding sphere
// against the near plane and the side planes of the current projection.
#include "SceneObjects.h"
#include "DrawContext.h"
#include "StaticMesh.h"
#include "Light.h"
#include "EAGLTransform.h"
extern "C" float sqrtf(float);
void DebugMsg(const char *, ...);

namespace EAGL {
class ViewPort {
  public:
    static MATRIX4 *gpProjectionMatrix;
};
}
namespace EALA {
namespace Character {
class Skeleton {
  public:
    void PrepForSkinning();
};
class Mesh {
  public:
    void Draw(CDrawContext &, bool);
};
}
}
// The sphere is visible when any of its distances to the near plane and the four side planes
// is within the radius. The out parameter gets the view depth.
int CAnimObject::ClipSphere(const MATRIX4 &matrix, const COORD3 &center, float nearClip, float radius, float &depth) {
    COORD3 p;
    EAGL::Transform view;
    EAGL::Transform *transform = &view;
    transform->BuildMatrix(&matrix);
    transform->TransformPoint(center, p);
    depth = p.z;
    float d = p.z - nearClip;
    if (!(d < -radius) && d <= radius)
        return 1;
    float slope = EAGL::ViewPort::gpProjectionMatrix->m[0][0];
    float scale = 1.0f / sqrtf(slope * slope + 1.0f);
    d = scale * (slope * p.x - p.z);
    if (!(d < -radius) && d <= radius)
        return 1;
    d = scale * (-slope * p.x - p.z);
    if (!(d < -radius) && d <= radius)
        return 1;
    slope = EAGL::ViewPort::gpProjectionMatrix->m[1][1];
    scale = 1.0f / sqrtf(slope * slope + 1.0f);
    d = scale * (slope * p.y - p.z);
    if (!(d < -radius) && !(d > radius))
        return 1;
    d = scale * (-slope * p.y - p.z);
    if (!(d < -radius) && d <= radius)
        return 1;
    return 0;
}

// The command a draw bin stores; only what this function writes is named.
struct DrawCommand {
    int sortKey;
    void *field_4;
    void *field_8;
    void *parameters[3]; // +0x0c, handed to the execute function
    int unknown_18;
    void (*execute)(CDrawContext &, void **);
};

struct DrawContextView {
    struct View {
        unsigned char unknown00[56];
        float nearClip; // +0x38
    };
    unsigned char unknown00[192];
    MATRIX4 *viewMatrix;
    unsigned char unknownc4[212 - 196];
    View *view;        // +0xd4
};

static inline void Transform(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }

static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(CMatrix::s_TempMat, o);
}

void CAnimObject::Draw(CDrawContext &context) {
    DrawContextView &view = (DrawContextView &)context;
    if (!poseEvaluated) {
        DebugMsg("CAnimObject::Draw(): animation for 0x%08x has not been updated.\n                     Skipping draw that would result in a frame of still pose.\n\n", this);
        return;
    }
    CMatrix *saved = context.modelView;
    context.modelView = &transform;
    void (*execute)(CDrawContext &, void **) = DrawMesh;
    DrawCommand command;
    StaticMeshPart *part = mesh->part;
    command.sortKey = (int)part;
    command.field_4 = this;
    command.execute = execute;
    command.parameters[0] = this;
    command.parameters[1] = mesh;
    command.parameters[2] = (void *)1;
    context.AddCommand((ERenderBins)5, command);
    int flag = mesh->noSortedPass;
    if (!flag) {
        const MATRIX4 *viewMatrix = (const MATRIX4 *)&view.viewMatrix;
        COORD3 center;
        center.x = part->center[0];
        center.y = part->center[1];
        center.z = part->center[2];
        float radius = part->radius;
        CMatrix m;
        CopyMatrix(m, JointMatrices()[1]);
        PostMultiply(m, transform);
        CVector3 origin(0.0f, 0.0f, 0.0f);
        CVector3 t;
        ::Transform(t, m, origin);
        center.x = t.x;
        center.y = t.y;
        center.z = t.z;
        float depth;
        ClipSphere(*viewMatrix, center, view.view->nearClip, radius, depth);
        context.modelView = &transform;
        depth *= depth;
        const float limit = 1000000.0f;
        if (!(depth <= limit))
            depth = limit;
        depth *= 256.0f;
        DrawCommand command2;
        command2.sortKey = (int)depth;
        command2.field_4 = part;
        command2.execute = execute;
        command2.field_8 = this;
        command2.parameters[0] = this;
        command2.parameters[1] = mesh;
        command2.parameters[2] = (void *)flag;
        context.AddCommand((ERenderBins)9, command2);
    }
    context.modelView = saved;
}

void CAnimObject::DrawMesh(CDrawContext &context, void **parameters) {
    CAnimObject *self = (CAnimObject *)parameters[0];
    EALA::Character::Mesh *mesh = (EALA::Character::Mesh *)parameters[1];
    bool sorted = parameters[2] != 0;
    if (self) {
        ((EALA::Character::Skeleton *)self->skeleton)->PrepForSkinning();
        if (mesh) {
            BPDLightVolume *volume = ((CAnimObjectInterface *)self)->GetLightVolume();
            CLight::SetLightBlock(volume, &context, ((CAnimObjectInterface *)self)->FadeValue());
            mesh->Draw(context, sorted);
        }
    }
}
