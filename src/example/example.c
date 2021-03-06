/*******************************************************************************
 Copyright (C) 2013  Trey Marc ( a t ) gmail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 -2013.12.18 : demo code

 ****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../utils/utils.h"
#include "../mwi/mwi.h"

HANDLE serialLink = NOK;
int initOk = NOK;

void callBack_mwi(int state);

int main(int argc, char* argv[])
{
    // serial devices "COM5" or /dev/ttyUSB0 ..
    char serialDevice[256] = "";

    for (int i = 1; i < argc; i++) {
        if (i + 1 != argc) {
            if (strcmp(argv[i], "-s") == 0) {
                strcpy(serialDevice, argv[i + 1]);
                i++;
            }
        }
    }

    MW_TRACE("starting..\n")
    serialLink = MWIserialbuffer_init(serialDevice, SERIAL_115200_BAUDRATE);

    if (serialLink == NOK) {
        perror("error opening serial port");
        exit(EXIT_FAILURE);
    }

    // mwi state
    mwi_mav_t *mwiState;
    mwiState = calloc(sizeof(*mwiState), sizeof(*mwiState));
    mwiState->callback = &callBack_mwi;

    uint64_t lastFrameRequest = 0;
    uint64_t currentTime = microsSinceEpoch();

    msp_payload_t *payload;
    payload = calloc(sizeof(*payload), sizeof(*payload));
    for (;;) {

        currentTime = microsSinceEpoch();

        if ((currentTime - lastFrameRequest) > 1000 * 30) {
            if (initOk == OK) {
                lastFrameRequest = currentTime;
                MWIserialbuffer_askForFrame(serialLink, MSP_RAW_IMU, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_DEBUG, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_ANALOG, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_ALTITUDE, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_COMP_GPS, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_RAW_GPS, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_RC, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_MOTOR, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_SERVO, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_RAW_IMU, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_STATUS, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_ATTITUDE, payload);
            } else {
                MWIserialbuffer_askForFrame(serialLink, MSP_IDENT, payload);
                MWIserialbuffer_askForFrame(serialLink, MSP_PRIVATE, payload);
            }
        }

        MWIserialbuffer_readNewFrames(serialLink, mwiState);

        usleep(5000);
    }
}

void callBack_mwi(int state)
{
    //  do something with the decode message
    switch (state) {
        case MSP_IDENT:
            initOk = OK;
            break;

        case MSP_STATUS:
            break;

        case MSP_RAW_IMU:
            break;

        case MSP_SERVO:
            break;

        case MSP_MOTOR:
            break;

        case MSP_RC:
            break;

        case MSP_RAW_GPS:
            break;

        case MSP_COMP_GPS:
            break;

        case MSP_ATTITUDE:
            break;

        case MSP_ALTITUDE:
            break;

        case MSP_ANALOG:
            break;

        case MSP_RC_TUNING:
            break;

        case MSP_ACC_CALIBRATION:
            break;

        case MSP_MAG_CALIBRATION:
            break;

        case MSP_PID:
            break;

        case MSP_BOX:
            break;

        case MSP_MISC:
            break;

        case MSP_MOTOR_PINS:
            break;

        case MSP_DEBUG:
            break;

        case MSP_BOXNAMES:
            break;

        case MSP_PIDNAMES:
            break;

        case MSP_PRIVATE:
            break;

        case NOK:
            break;
    }

}
