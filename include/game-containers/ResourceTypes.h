// Scoped resource-prefix and container-operation models. See docs/GameAlgorithms.md.
// Pointees remain incomplete. Pointer operations view only the observed Resource prefix.
// Pointer<Resource> models an unknown managed pointee in Context/SubTaskInfo;
// it does not assert the original member specialization or the full pointee layout.
#ifndef MOH_RESOURCE_CONTAINER_TYPES_H
#define MOH_RESOURCE_CONTAINER_TYPES_H
#pragma interface
#include <string>
extern "C" int strcasecmp(const char *, const char *);
namespace EALA {
class Resource {
  public:
    int field_00;
    Resource() : field_00(0) {}
    virtual ~Resource() {} // Inlined in derived destructors; the weak copy is at 0x8027c918.
    void Release() {
        if (--field_00 == 0)
            delete this;
    }
    template <class T> class Pointer {
        T *value;

      public:
        Pointer() : value(0) {}
        Pointer(T *p) : value(p) {
            if (value)
                ++reinterpret_cast<Resource *>(value)->field_00;
        }
        Pointer(const Pointer &other) : value(other.value) {
            if (value)
                ++reinterpret_cast<Resource *>(value)->field_00;
        }
        ~Pointer() {
            if (value)
                reinterpret_cast<Resource *>(value)->Release();
        }
        Pointer &operator=(const Pointer &other) {
            if (value != other.value) {
                T *previous = value;
                value = other.value;
                if (value)
                    ++reinterpret_cast<Resource *>(value)->field_00;
                if (previous)
                    reinterpret_cast<Resource *>(previous)->Release();
            }
            return *this;
        }
        bool IsValid() const { return value != 0; } // Descriptive name; the compiled test yields a bool.
        T *operator->() const { return value; }
        operator T *() const { return value; }
    };
};
class EAGLLoader;
class Task;
namespace Character {
class State;
class Opcode;
class Skeleton {
  public:
    class Partition {
      public:
        unsigned char unresolved_00[36] __attribute__((aligned(4)));
        int field_24;
    };
};
class Choreographer : public Resource {
  public:
    virtual ~Choreographer();
    virtual void EvalPose(float time);
    virtual void SetState(const char *name, EALA::Task *done, float blend, const Skeleton::Partition *partition);
    virtual void SetAuthoredTransitionState(const char *name);
    virtual void SetAuthoredTransitionState(Resource::Pointer<State> &state);
    virtual Resource::Pointer<State> GetAuthoredTransitionState();
    static Choreographer *Current();
    void HandleMessage();
    void HandleOpcode(Opcode &opcode);
    void HandleRotation(float rotation);
    void HandleCycleEnd();
    void HandleAnimDuration(float duration);
    struct Context {
        Resource::Pointer<Resource> field_00;
        unsigned int field_04, field_08;
        float field_0c;
    };
    struct PartitionCompare {
        bool operator()(const Skeleton::Partition *a, const Skeleton::Partition *b) const {
            int av = 0;
            if (a)
                av = a->field_24;
            int bv = 0;
            if (b)
                bv = b->field_24;
            if (av < bv)
                return true;
            if (bv < av)
                return false;
            return a < b;
        }
    };
};
class MatrixBlend {
  public:
    struct SubTaskInfo {
        float field_00, field_04;
        Resource::Pointer<Resource> field_08;
    };
};
class LinearBlend {
  public:
    struct SubTaskInfo {
        float field_00, field_04;
        Resource::Pointer<Resource> field_08;
    };
};
} // namespace Character
} // namespace EALA
struct StringCompare {
    bool operator()(const _STL::string &a, const _STL::string &b) const {
        return strcasecmp(a.c_str(), b.c_str()) < 0;
    }
};

typedef char ResourcePrefixCheck[sizeof(EALA::Resource) == 8 ? 1 : -1];
typedef char
    ChoreographerContextCheck[sizeof(EALA::Character::Choreographer::Context) == 16 ? 1 : -1];
typedef char MatrixSubTaskCheck[sizeof(EALA::Character::MatrixBlend::SubTaskInfo) == 12 ? 1 : -1];
typedef char LinearSubTaskCheck[sizeof(EALA::Character::LinearBlend::SubTaskInfo) == 12 ? 1 : -1];
#endif
