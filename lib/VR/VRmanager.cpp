#include "VRmanager.h"

// RX:D2, TX:D1
VR vrSerial(D7, D6);


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
    if (op_mode == RECOGNIZER_MODE) {
        recognizerMode();
    }
    else if (op_mode == TRAINER_MODE) {
        trainerMode();
    }
}


/**
 * Recognizer Mode
 * */

void VRmanager::recognizerMode()
{

}


/**
 * Trainer Mode
 * */

void VRmanager::trainerMode()
{

}