// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
class TriggerObject;
void *CreateDestructable(TriggerObject *, int, int);

void *CreatePowerup(TriggerObject *trigger, int index, int value) { return CreateDestructable(trigger, index, value); }
