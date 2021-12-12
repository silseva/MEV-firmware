
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "miosix.h"

#include "Bed/ValveController.h"
#include "Bed/SensorSampler.h"
#include "Bed/UI/UiFsmData.h"
#include "common/Fsm.h"

using namespace std;
using namespace mxgui;

StateData state;

int main()
{
    memset(&state, 0x00, sizeof(StateData));

    SensorSampler sampler;
    sampler.start();

    DrawingContext dc(DisplayManager::instance().getDisplay());
    BedFsmData UiFsm(state);
    Fsm uiFsm(&UiFsm.mainPage, 50);
    uiFsm.start();

    ValveController vc(state);
    vc.run();

    return 0;
}
