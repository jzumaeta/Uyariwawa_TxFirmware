#include "VRtrainer.h"
#include "VRmanager.h"


bool VRtrainer::set_command()
{

}

String inputString = "";

void VRtrainer::loop()
{
    while (vrSerial.available()) {
        inputString += (char)vrSerial.read();
        if (inputString.endsWith("\n")) {
        }
    }
}


bool cmd_validation()
{
    if (0) {

    }

    return false;
}