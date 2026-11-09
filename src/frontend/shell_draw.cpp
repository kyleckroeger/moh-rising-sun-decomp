// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::Draw() {
    engine->Render();
    DrawUnplugged();
}
void ShellManager::DrawUnplugged() {}
int ShellManager::GetPackageData(const char *name, unsigned char **data, bool &owned) { return loader->LoadPackage(name, data, owned); }
int ShellManager::LoadResources(FEPackage *package, long count, FEResourceRequest *requests) { return loader->LoadResources(package, count, requests); }
int ShellManager::UnloadResources(FEPackage *package, long count, FEResourceRequest *requests) { return loader->UnloadResources(package, count, requests); }
