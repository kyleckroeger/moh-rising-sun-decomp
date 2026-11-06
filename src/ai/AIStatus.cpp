// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. The variable and comparison meanings are inferred from their uses.
#include "AITargeting.h"

CAIStatus::CAIStatus() {}

CAIStatus::~CAIStatus() {}

int CAIStatus::MatchStatus(aistatus_var var, int target, aistatus_comparison comparison) {
    int value = 0;
    switch (var) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 11:
        value = values[var];
        break;
    case 5:
        value = s_paioAttacker->ComputeStatusDistance(s_pTarget);
        break;
    case 7:
        value = s_paioAttacker->ComputeStatusThreatValue(s_pTarget);
        break;
    case 8:
        value = s_paioAttacker->ComputeStatusLOSValue(s_pTarget);
        break;
    case 9:
        value = s_paioAttacker->ComputeStatusExponentialDistance(s_pTarget);
        break;
    case 10:
        value = s_paioAttacker->ComputeStatusIsEnemy(s_pTarget);
        break;
    }
    switch (comparison) {
    case 0:
        return value == target;
    case 1:
        if (value != target)
            return 1;
        return 0;
    case 2:
        return value < target;
    case 3:
        return value > target;
    }
    return 0;
}

int CAIStatus::MatchStatus(aistatus_match *match) {
    return MatchStatus(match->var, match->value, match->comparison);
}

int CAIStatus::MatchStatusList(aistatus_match *match, int count) {
    for (; count > 0; count--, match++) {
        if (!MatchStatus(match))
            return 0;
    }
    return 1;
}
