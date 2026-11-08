// CSkyBox: the singleton's storage and lifetime, its update and its draw (the mesh drawn around
// the camera's position, followed by a command that clears the depth buffer).
#include "SkyBox.h"

extern unsigned char SkyBoxSingletonBuf[];

void *CSkyBox::operator new(unsigned int size) {
    msbAllocated = 1;
    return SkyBoxSingletonBuf;
}

void CSkyBox::operator delete(void *p) {
    msbAllocated = 0;
}

void CSkyBox::InitClass() {
    mspInitialized = 1;
}

void CSkyBox::ShutdownClass() {
    if (mspSingleton)
        DestroyInstance();
    mspInitialized = 0;
}

void CSkyBox::CreateInstance(CScreen &screen) {
    mspSingleton = new CSkyBox(screen);
}

CSkyBox *CSkyBox::GetInstance() {
    return mspSingleton;
}

void CSkyBox::DestroyInstance() {
    if (mspSingleton)
        delete mspSingleton;
    mspSingleton = 0;
}

struct ClearZBufferContextView {
    unsigned char unknown[548];
    struct Table {
        unsigned char unknown[32];
        short delta;
        void (*clear)(void *);
    } *table;
};

static void ClearZBufferCommand(CDrawContext &context, void **) {
    ClearZBufferContextView *view = (ClearZBufferContextView *)&context;
    view->table->clear((char *)&context + view->table->delta);
}

void CSkyBox::BeginUpdate(float elapsed) {
    mesh.Update(elapsed);
}

static inline void Invert(CMatrix &m, const CMatrix &from) {
    m = from;
    CMatrix::s_TempMat = m;
    m.FastInverse(CMatrix::s_TempMat);
}

struct SkyBoxDrawCommand {
    int word00;
    unsigned char unknown04[24];
    void (*function)(CDrawContext &, void **);
};

void CSkyBox::Draw(CDrawContext &context) {
    if (enabled) {
        CMatrix *saved = context.modelView;
        CMatrix transform;
        CMatrix *p = &transform;
        *p = mesh.transform;
        CMatrix inverse;
        Invert(inverse, *context.camera);
        CVector3 position = inverse.GetPos();
        p->Translate(position);
        context.modelView = p;
        mesh.Draw(context, 0, 1, (ERenderBins)0, (ERenderBins)1);
        SkyBoxDrawCommand command;
        command.word00 = 0;
        command.function = ClearZBufferCommand;
        context.AddCommand((ERenderBins)2, *(const DrawCommand *)&command);
        context.modelView = saved;
    }
}
