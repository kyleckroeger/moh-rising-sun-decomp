// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline's jaw
// controller as a semantic reference. The class, its methods and EJawType are named
// by the mangled symbols; member names are descriptive inferences from the offsets.
#include "SceneObjects.h"
unsigned int MathFunRandomUI32Raw();

void CJawController::SetMotionType(EJawType newType) {
    type = newType;
    switch (type) {
    case 5:
        riseTime = 0.4f;
        holdTime = 1.0f;
        fallTime = 5.0f;
        minValue = 0.0f;
        maxValue = 1.0f;
        cycleCount = 10;
        previousCode = currentCode;
        currentCode = 6;
        cyclesLeft = cycleCount;
        timer = startDelay = 2.0f;
        delaying = 1;
        break;
    case 6:
        riseTime = 6.0f;
        holdTime = 8.0f;
        fallTime = 4.0f;
        minValue = 0.0f;
        maxValue = 0.4f;
        cycleCount = MathFunRandomUI32Raw() % 5 + 5;
        previousCode = currentCode;
        currentCode = 1;
        cyclesLeft = cycleCount;
        timer = startDelay = 2.0f;
        delaying = 1;
        break;
    case 2:
        riseTime = 0.6f;
        holdTime = 0.4f;
        fallTime = 0.2f;
        minValue = 0.1f;
        maxValue = 0.5f;
        cycleCount = MathFunRandomUI32Raw() % 10 + 5;
        previousCode = currentCode;
        currentCode = 1;
        cyclesLeft = cycleCount;
        timer = startDelay = 2.0f;
        delaying = 1;
        break;
    case 3:
        riseTime = 0.4f;
        holdTime = 0.4f;
        fallTime = 0.1f;
        minValue = 0.15f;
        maxValue = 0.5f;
        cycleCount = MathFunRandomUI32Raw() % 10 + 5;
        previousCode = currentCode;
        currentCode = 9;
        cyclesLeft = cycleCount;
        timer = startDelay = 1.0f;
        delaying = 1;
        break;
    case 7:
        riseTime = 0.4f;
        holdTime = 0.4f;
        fallTime = 0.1f;
        minValue = 0.15f;
        maxValue = 0.5f;
        cycleCount = 10;
        previousCode = currentCode;
        currentCode = 9;
        cyclesLeft = cycleCount;
        timer = startDelay = 0.1f;
        delaying = 1;
        break;
    case 4:
        riseTime = 0.2f;
        holdTime = 0.5f;
        fallTime = 0.0f;
        minValue = 0.2f;
        maxValue = 0.8f;
        cycleCount = 6;
        previousCode = currentCode;
        currentCode = 9;
        cyclesLeft = cycleCount;
        timer = startDelay = 1.5f;
        delaying = 1;
        break;
    case 1:
        riseTime = 0.15f;
        holdTime = 5.0f;
        fallTime = 0.0f;
        minValue = 0.0f;
        maxValue = 0.7f;
        cycleCount = 1;
        previousCode = currentCode;
        currentCode = 9;
        cyclesLeft = cycleCount;
        timer = startDelay = 0.0f;
        delaying = 1;
        break;
    default:
        riseTime = 0.0f;
        holdTime = 86400.0f;
        fallTime = 0.0f;
        minValue = 0.0f;
        maxValue = 0.0f;
        cycleCount = 1;
        previousCode = currentCode;
        currentCode = 9;
        cyclesLeft = cycleCount;
        timer = startDelay = 0.0f;
        delaying = 0;
        break;
    }
}
