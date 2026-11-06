// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. A fragment of the original AIObject.cpp; the helper name is descriptive.
#include "AITargeting.h"

// Whether a team treats the given team as a valid target.
int CAIObject::IsValidTargetTeam(unsigned char other) {
    switch (team) {
    case 1:
        switch (other) {
        case 0:
        case 1:
            return 0;
        case 2:
            return 1;
        }
        break;
    case 2:
        switch (other) {
        case 0:
        case 2:
            return 0;
        case 1:
            return 1;
        }
        break;
    case 0:
        switch (other) {
        case 0:
            return 0;
        case 1:
        case 2:
            return 1;
        }
        break;
    }
    return 0;
}
