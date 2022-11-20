
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "miosix.h"
#include "mxgui/display.h"

#include "BellJar/UI/UiFsmData.h"
#include "BellJar/LevelController.h"
#include "BellJar/BjState.h"
#include "BellJar/settings.h"

using namespace std;
using namespace mxgui;

BjState bjState;

int main()
{
    if(loadParamsFromFlash(bjState.ctParams) == false)
    {
        // Loading saved parameters went wrong, initialize them to safe values
        bjState.ctParams.uMin    = 0.0f;
        bjState.ctParams.uMax    = 1.0f;
        bjState.ctParams.k       = 0.0f;
        bjState.ctParams.Ti      = 0.0f;
        bjState.ctParams.Tsample = 0.0f;
    }

    // Initialize all the other non-persistent parameters
    bjState.ctMode     = CtrlMode::MAN;
    bjState.ctSetPoint = 0.0f;
    bjState.manOutput  = 0.0f;
    bjState.zeroLevel  = 0;
    bjState.maxLevel   = 4095;

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
