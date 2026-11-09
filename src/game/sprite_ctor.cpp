// CSprite's constructors and destructor. Member names follow what the code does with them.
struct ShapeFile;
namespace EAGL { class TAR { public: ~TAR(); }; }

void *TLT_LoadFileFromLevelBigFile(const char *, int *);
void *TLT_LoadFileNormal(const char *, int *, int);
void TLT_CloseFile(void *);
void DebugMsg(const char *, ...);

class CSprite {
public:
    enum ESpriteLoadType { LoadTypeUnknown = 0 };
    CSprite(ShapeFile *shape);
    CSprite(const char *name, ESpriteLoadType type);
    ~CSprite();
    void Init(ShapeFile *shape);

    ShapeFile *file;
    EAGL::TAR *texture;
    int width;
    int height;
    float scaleX;
    float scaleY;
    int ownsTexture;
    int field1c;
    int field20;
};

CSprite::CSprite(ShapeFile *shape) {
    Init(shape);
}

CSprite::CSprite(const char *name, ESpriteLoadType type) {
    file = 0;
    texture = 0;
    field1c = 0;
    field20 = 0;
    if (name) {
        int size;
        void *loaded;
        switch (type) {
        case 1:
        default:
            loaded = TLT_LoadFileFromLevelBigFile(name, &size);
            break;
        case 2:
            loaded = TLT_LoadFileNormal(name, &size, 0);
            break;
        }
        if (!loaded) {
            DebugMsg("CSprite::CSprite(%s):  TLT_LoadFile... FAILED!!!\n", name);
        } else {
            texture = 0;
            file = (ShapeFile *)loaded;
            Init((ShapeFile *)loaded);
        }
    }
}

CSprite::~CSprite() {
    if (file) {
        TLT_CloseFile(file);
        file = 0;
    }
    if (ownsTexture && texture)
        delete texture;
}
