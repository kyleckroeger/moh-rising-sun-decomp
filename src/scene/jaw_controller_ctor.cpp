// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline's jaw
// controller as a semantic reference. The class, its methods and EJawType are named
// by the mangled symbols; member names are descriptive inferences from the offsets.
#include "SceneObjects.h"

CJawController::CJawController()
    : closeStart(0.0f), closeEnd(0.0f), riseTime(0.0f), holdTime(0.0f), fallTime(0.0f), cyclesLeft(0),
      cycleCount(0), minValue(0.0f), maxValue(0.0f), previousCode(-1), currentCode(0), timer(0.0f),
      value(0.0f), startDelay(0.0f), delaying(1), type(0) {
}
