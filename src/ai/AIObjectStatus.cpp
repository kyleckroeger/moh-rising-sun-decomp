// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

void *CAIObject::AsAISoldierObject() { return 0; }
void *CAIObject::AsAIPlayerObject() { return 0; }
void *CAIObject::AsAITankObject() { return 0; }
void CAIObject::OnStateChange(EAIHighLevelState, EAIHighLevelState) {}
void CAIObject::OnDeath() {}
void CAIObject::SetupTargetMatchList(aistatus_match *, int) {}
bool CAIObject::IsCrouching() const { return false; }
int CAIObject::IsPlayer() const { return 0; }

int CAIObject::CheckIncomingFlagAll(unsigned int flags) const { return (statusFlags & flags) == flags; }

int CAIObject::GetAIStatusVar(aistatus_var var) const { return status.GetVarValue(var); }

void CAIObject::SetAIStatusVar(aistatus_var var, int value) {
    int previous = status.GetVarValue(var);
    status.SetVarValue(var, value);
    if (var == 11 && value == 15 && previous != 15)
        ((CAIObjectInterface *)this)->OnDeath();
}
