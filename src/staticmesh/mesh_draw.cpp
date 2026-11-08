// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp. Draw queues a command per mesh part; the second part is
// sorted by its distance from the camera. DrawMesh runs one command.
#include "Light.h"
#include "DrawContext.h"
#include "StaticMesh.h"
#include "EAGLTransform.h"
#include "SceneObjects.h"

extern float g_NearClip;

namespace EAGL {
class Model {
  public:
    class Variable {
      public:
        bool IsModifiable() const;
        unsigned char unknown00[12];
        void *data; // +0x0c
    };
    class VariableTable {
      public:
        struct Iterator {
            Variable *variable;
        };
        Iterator find_first(const char *) const;
        Iterator find_end(const char *) const;
    };
    void Draw(const MATRIX4 &);
};
}

// The command a draw bin stores; only what these functions write is named.
struct DrawCommand {
    int sortKey;
    CStaticMesh *mesh;
    int unknown08;
    void *parameters[4]; // +0x0c, handed to the execute function
    void (*execute)(CDrawContext &, void **);
};

struct StaticMeshParts {
    unsigned char unknown00[32];
    void *parts[2]; // +0x20
};

// The last computation index set on each part; CStaticMesh's constructor starts them at -1.
struct MeshPartModes {
    unsigned char unknown00[16];
    int modes[2]; // +0x10
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
    unsigned char unknownd8[252 - 216];
    IMovingSceneNode *node; // +0xfc
};

static inline void GetSphereCenter(COORD3 &center, const StaticMeshPart &part) {
    center.x = part.center[0];
    center.y = part.center[1];
    center.z = part.center[2];
}

// The depth of a point in view space.
static inline float ViewDepth(const COORD3 &point, const MATRIX4 *matrix) {
    EAGL::Transform transform;
    transform.BuildMatrix(matrix);
    COORD3 out;
    transform.TransformPoint(point, out);
    return out.z;
}

void CStaticMesh::Draw(CDrawContext &context, int, unsigned int, ERenderBins bin0, ERenderBins bin1) {
    DrawContextView &view = (DrawContextView &)context;
    int mode = 3;
    RebindAnimatedTextures();
    g_NearClip = view.view->nearClip;
    if (lighting)
        mode = 4;
    if (mesh->part0) {
        DrawCommand command;
        command.sortKey = (int)mesh->part0;
        command.mesh = this;
        command.execute = DrawMesh;
        command.parameters[2] = view.node;
        command.parameters[3] = 0;
        command.parameters[0] = this;
        command.parameters[1] = (void *)mode;
        context.AddCommand(bin0, command);
    }
    StaticMeshPart *part = mesh->part1;
    if (part) {
        COORD3 center;
        GetSphereCenter(center, *part);
        float distance = ViewDepth(center, (const MATRIX4 *)&view.viewMatrix);
        distance *= distance;
        const float limit = 1000000.0f;
        if (!(distance <= limit))
            distance = limit;
        distance *= 256.0f;
        DrawCommand command;
        command.execute = DrawMesh;
        command.parameters[0] = this;
        command.sortKey = (int)distance;
        command.parameters[1] = (void *)mode;
        command.parameters[2] = view.node;
        command.parameters[3] = (void *)1;
        context.AddCommand(bin1, command);
    }
}

void CStaticMesh::DrawMesh(CDrawContext &context, void **parameters) {
    CStaticMesh *self = (CStaticMesh *)parameters[0];
    int index = (int)parameters[3];
    IMovingSceneNode *node = (IMovingSceneNode *)parameters[2];
    EAGL::Model *model = (EAGL::Model *)((StaticMeshParts *)self->mesh)->parts[index];
    const MATRIX4 *matrix = ((DrawContextView &)context).viewMatrix;
    if (node) {
        BPDLightVolume *volume = node->GetLightVolume();
        CLight::SetLightBlock(volume, &context, ((CAnimObjectInterface *)node)->FadeValue());
    }
    int mode = (int)parameters[1];
    if (mode != ((MeshPartModes *)self)->modes[index]) {
        ((MeshPartModes *)self)->modes[index] = mode;
        EAGL::Model::VariableTable *table = (EAGL::Model::VariableTable *)model;
        EAGL::Model::VariableTable::Iterator it = table->find_first("ComputationIndex::ComputationIndex");
        EAGL::Model::VariableTable::Iterator end = table->find_end("ComputationIndex::ComputationIndex");
        for (EAGL::Model::Variable *variable = it.variable; variable != end.variable; variable++) {
            short value = mode;
            short *data = 0;
            bool modifiable;
            if (variable->IsModifiable()) {
                data = (short *)variable->data;
                modifiable = true;
            } else
                modifiable = false;
            if (modifiable)
                *data = value;
        }
    }
    model->Draw(*matrix);
}
