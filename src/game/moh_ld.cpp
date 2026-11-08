// AI-assisted reconstruction from GR8E69. Part of the original drivecheck.cpp.
#include <stdio.h>

extern "C" long DVDConvertPathToEntrynum(const char *);

bool LevelExistsOnCurrentDisc(int mission, int stage) {
    char path[80];
    sprintf(path, "data/%d/%d_%d/comp.viv", mission, mission, stage);
    return DVDConvertPathToEntrynum(path) != -1;
}
