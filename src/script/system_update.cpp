// AI-assisted reconstruction from GR8E69; see docs/Script.md.
int BSEventProcessEvents(int);
int BSProcessMessages(int);
int BSUpdateBIFuncs();
void BSUpdateTimer(float);
void UpdateProximityTriggerStatus();
void UpdateObjectCreation();

void BSUtilUpdate(float delta) {
    // Preserve the comparison direction: unordered values also select the cap.
    if (!(delta <= 6.0f)) delta = 6.0f;
    BSEventProcessEvents(256);
    BSProcessMessages(1024);
    BSUpdateBIFuncs();
    BSUpdateTimer(delta);
    UpdateProximityTriggerStatus();
    UpdateObjectCreation();
}
