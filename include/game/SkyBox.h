#ifndef SKYBOX_H
#define SKYBOX_H
// A scoped view of the sky box, a single scene node holding one static mesh.
// Method names come from the original symbols; member names follow the code.
#include "CMatrix.h"
#include "SceneNode.h"
#include "Screen.h"

#include "DrawContext.h"

class CStaticMesh {
public:
    ~CStaticMesh();
    void Update(float elapsed);
    void Draw(CDrawContext &context, int, unsigned int, ERenderBins, ERenderBins);

    unsigned char unknown00[32];
    CMatrix transform;
};

class CSkyBox : public ISceneNode {
public:
    virtual ~CSkyBox();
    void *operator new(unsigned int size);
    void operator delete(void *p);
    CSkyBox(CScreen &screen);
    static void InitClass();
    static void ShutdownClass();
    static void CreateInstance(CScreen &screen);
    static CSkyBox *GetInstance();
    static void DestroyInstance();
    virtual void BeginUpdate(float elapsed);
    virtual void Draw(CDrawContext &context);
    virtual int IsVisible(CDrawContext &context) const;
    virtual unsigned int IsDrawEnabled() const;

    unsigned char unknown18[0x38 - 0x18];
    CScreen *screen;
    int enabled;
    CStaticMesh mesh;

    static CSkyBox *mspSingleton;
    static int mspInitialized;
    static int msbAllocated;
};

#endif
