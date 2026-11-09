// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "profile.h"
#include <map>
#include <string>
#include "Weapon.h"
#include "StaticMesh.h"
extern "C" void *DWI_alloc(const char *, int, int);
void *TLT_LoadFromRepository(const char *, int *, bool *);
inline void *operator new[](unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
class CWeaponFactory {
  public:
    _STL::map<int, WeaponProperties_struct *> properties; // +0x00
    CStaticMesh *meshes;                                  // +0x10
    void LoadWeaponObjects();
};
void CWeaponFactory::LoadWeaponObjects() {
    typedef _STL::map<_STL::string, CStaticMesh *> MeshMap;
    typedef _STL::map<int, WeaponProperties_struct *>::iterator Iterator;
    MeshMap unique;
    Iterator it = properties.begin();
    for (; it != properties.end(); ++it) {
        _STL::string name((*it).second->modelName);
        if (unique.find(name) == unique.end()) unique[name] = 0;
    }
    meshes = new ("source/weapons/weaponfactory.cpp:433") CStaticMesh[unique.size()];
    int next = 0;
    for (it = properties.begin(); it != properties.end(); ++it) {
        _STL::string name((*it).second->modelName);
        MeshMap::iterator found = unique.find(name);
        if (!(*found).second) {
            bool inRepository;
            void *data = TLT_LoadFromRepository((*it).second->modelName, 0, &inRepository);
            if (data) {
                meshes[next].Init(data, !inRepository);
                (*found).second = &meshes[next];
                (*it).second->model = &meshes[next];
                next++;
            }
        } else {
            (*it).second->model = (*found).second;
        }
    }
}
