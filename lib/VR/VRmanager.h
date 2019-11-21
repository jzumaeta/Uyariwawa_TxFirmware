#include <Arduino.h>
#include "VRLib/VoiceRecognitionV3.h"


class VRmanager
{
    enum mode_t {
        RECOGNIZER_MODE = 0,
        TRAINER_MODE
    };
    
public:
    VRmanager();
    void Init();
    void SetMode(mode_t mode);
    void Loop();    

private:
    void recognizerMode();
    void trainerMode();

private:
    mode_t op_mode = RECOGNIZER_MODE; //Operation Mode
};

extern VR vrSerial;