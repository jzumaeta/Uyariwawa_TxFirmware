#include "VRmanager.h"

// RX:D2, TX:D1
VR vrSerial(D2, D1);


void VRmanager::Init()
{
    vrSerial.begin(9600);
}

void VRmanager::SetMode(mode_t mode)
{
    //check for waiting jobs
    //stop tickes and change handles...
    op_mode = mode;
}

void VRmanager::Loop()
{
    //check operation mode
    if (op_mode == MODE_RECOGNIZER) {

    }
    else if (op_mode == MODE_TRAINER) {

    }
    else {
        //Unknow state
        Serial.println("[VR_ERROR] Unknow operation state! Reset...");
        delay(250);
        ESP.restart();
    }
}