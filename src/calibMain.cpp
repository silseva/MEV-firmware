/*
 * MEV board firmware
 * Copyright (C) 2021 - 2024  Silvano Seva silvano.seva@polimi.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "miosix.h"
#include "mxgui/display.h"

#include "Bed/AnalogSensors.h"
#include "drivers/ADC122S021.h"
#include "drivers/hwmapping.h"

#include "drivers/calibration.h"

using namespace std;
using namespace mxgui;

static constexpr float SENS_SUPPLY_VOLTAGE = 5.0364f;
static constexpr float ADC_OFFSET[2] = {0.0f, 1.493074228f};
static constexpr float ADC_SLOPE[2]  = {813.4338894f, 813.1515922f};

auto& Adc = ADC122S021::instance();

static void sampleChannels()
{
    unsigned long long time = miosix::getTick();
    uint32_t values[2] = {0};
    uint32_t count = 0;

    // Select flow sensors inputs
    adc::muxs::mode(miosix::Mode::OUTPUT);
    adc::muxs::low();

    for(uint8_t i = 0; i < 10; i++)
    {
        values[0] += Adc.getRawValue(AdcChannel::_1);
        values[1] += Adc.getRawValue(AdcChannel::_2);
        count     += 1;

        // Sample at 20Hz
        time += 50;
        miosix::Thread::sleepUntil(time);
    }

    values[0] /= 10;
    values[1] /= 10;
    count      = 0;

    iprintf("%ld %ld\n", values[0], values[1]);
}

static void writeCal(adCal_t& cal)
{
    writeAnalogCalibrationData(cal);

    cal     = defaultAnalogCalibrationData();
    bool ok = readAnalogCalibrationData(cal);

    printf("%d %f %f %f %f %f\n", ok, cal.SENS_SUPPLY_VOLTAGE, cal.ADC_OFFSET[0],
                                  cal.ADC_OFFSET[1], cal.ADC_SLOPE[0],
                                  cal.ADC_SLOPE[1]);
}

static void inputCal(adCal_t& cal)
{
    printf("SENS_SUPPLY_VOLTAGE: ");
    scanf("%f", &cal.SENS_SUPPLY_VOLTAGE);

    printf("ADC_OFFSET[0]: ");
    scanf("%f", &cal.ADC_OFFSET[0]);

    printf("ADC_OFFSET[1]: ");
    scanf("%f", &cal.ADC_OFFSET[1]);

    printf("ADC_SLOPE[0]: ");
    scanf("%f", &cal.ADC_SLOPE[0]);

    printf("ADC_SLOPE[1]: ");
    scanf("%f", &cal.ADC_SLOPE[1]);
}

int main()
{

    adCal_t cal;
    cal.SENS_SUPPLY_VOLTAGE = SENS_SUPPLY_VOLTAGE;
    cal.ADC_OFFSET[0]       = ADC_OFFSET[0];
    cal.ADC_OFFSET[1]       = ADC_OFFSET[1];
    cal.ADC_SLOPE[0]        = ADC_SLOPE[0];
    cal.ADC_SLOPE[1]        = ADC_SLOPE[1];

    while(1)
    {
        char cmd = getchar();

        switch(cmd)
        {
            case 's':
                sampleChannels();
                break;

            case 'i':
                inputCal(cal);
                break;

            case 'p':
                printf("VSUPPLY: %f\nOFFSET 0: %f\nOFFSET 1: %f\nSLOPE 0: %f\nSLOPE 1: %f\n\n",
                       cal.SENS_SUPPLY_VOLTAGE, cal.ADC_OFFSET[0], cal.ADC_OFFSET[1],
                       cal.ADC_SLOPE[0], cal.ADC_SLOPE[1]);
                break;

            case 'm':
                Adc.setConversionParameters(AdcChannel::_1, cal.ADC_SLOPE[0], cal.ADC_OFFSET[0]);
                Adc.setConversionParameters(AdcChannel::_2, cal.ADC_SLOPE[1], cal.ADC_OFFSET[1]);
                printf("%f %f\n", Adc.getVoltage(AdcChannel::_1), Adc.getVoltage(AdcChannel::_2));
                break;

            case 'w':
                writeCal(cal);
                break;
        }

        puts("");
    }

    return 0;
}
