#include <Arduino.h>
#include "VoiceRecognitionV3.h"

enum mode_t {
    MODE_RECOGNIZER = 0,
    MODE_TRAINER
};


class VRmanager
{
public:
    VRmanager();
    void Init();
    void SetMode(mode_t mode);
    void Loop();
private:
    mode_t op_mode = MODE_RECOGNIZER; //Operation Mode
};

extern VR vrSerial;