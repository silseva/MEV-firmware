
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "miosix.h"

#include "Bed/ValveController.h"
#include "Bed/SensorSampler.h"
#include "Bed/AnalogSensors.h"
#include "Bed/UI/UiFsmData.h"
#include "common/Persistence.h"
#include "common/RingBuffer.h"
#include "common/Fsm.h"

using namespace std;
using namespace mxgui;
using namespace miosix;

StateData state;

int main()
{
    state.resetVolumes = true;
    state.enabled      = false;
    state.tIns         = 0.0f;
    state.IE           = 0.0f;
    state.Fsample      = 0.0f;

    state.cal.loadDefaultValues();

    if(loadDataFromFlash(&(state.cal), sizeof(SensorCalibration)) == true)
    {
        AnalogSensors::instance().applyCalibration(state.cal);
    }

    SensorSampler sampler;
    sampler.start();

    BedFsmData UiFsm(state);
    Fsm uiFsm(&UiFsm.mainPage, 50);
    uiFsm.start();

    ValveController vc(state);
    vc.start();

    while(1)
    {
        #ifndef LOG_PRINT
        if(getchar() == 'd')
        {
            loggerSample_t sample;
            while(state.log.pop(sample))
            {
                printf("%llu,%.3f,%.3f,%.3f,%.3f,%.3f,%d,%d\n",
                       sample.timestamp,       sample.pressure,
                       sample.flow1,           sample.flow2,
                       sample.volume1,         sample.volume2,
                      (sample.valves & 0x01), (sample.valves >> 1));
            }
        }
        #endif

        Thread::sleep(250);
    }

    return 0;
}
