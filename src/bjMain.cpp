
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "miosix.h"
#include "mxgui/display.h"

#include "BellJar/UI/UiFsmData.h"
#include "BellJar/LevelController.h"
#include "BellJar/BjState.h"

using namespace std;
using namespace mxgui;

BjState bjState;

int main()
{
    memset(&bjState, 0x00, sizeof(BjState));

    bjState.ctParams.uMin    = 0.0;
    bjState.ctParams.uMax    = 1.0;
    bjState.ctParams.k       = 1.0;
    bjState.ctParams.Ti      = 0.5;
    bjState.ctParams.Tsample = 0.5;
    bjState.ctMode           = CtrlMode::AUTO;
    bjState.ctSetPoint       = 0.3;

    bjState.zeroLevel = 0;
    bjState.maxLevel  = 4095;

    LevelController lc;
    lc.start();

    BjFsmData bjFsm;
    Fsm uiFsm(&bjFsm.mainPage, 50);
    uiFsm.start();

    unsigned long long time = miosix::getTick();

    while(1)
    {
        miosix::ledOn();
        printf("%.3f,%.3f,%.3f,%d,%d,%d\n", bjState.ctSetPoint, bjState.levelNorm,
                                            bjState.ctOutput,   bjState.levelRaw,
                                            bjState.zeroLevel,  bjState.maxLevel);
        miosix::ledOff();

        // Sample at 20Hz
        time += 50;
        miosix::Thread::sleepUntil(time);
    }

    return 0;
}
