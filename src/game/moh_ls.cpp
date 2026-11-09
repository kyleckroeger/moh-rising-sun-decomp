// AI-assisted reconstruction from GR8E69. Part of the original LoadImage.cpp.
int GetLoadscreenId(int multiplayer, bool second, bool first, bool none) {
    int id;
    if (multiplayer) {
        if (first)
            id = 4;
        else
            id = 2;
    } else {
        if (second)
            id = 3;
        else
            id = 1;
    }
    if (none)
        id = 0;
    return id;
}
