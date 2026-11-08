// AI-assisted reconstruction from GR8E69. Part of the original AIThreats.cpp.
int NoiseTypeToEvent(int type) {
    int event = 48;
    switch (type) {
    case 48:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 49:
        event = 49;
        break;
    }
    return event;
}
