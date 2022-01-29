#ifndef MIC_H
    #define MIC_H
        
        #include <Arduino.h>
       
        // Use these with the Teensy Audio Shield
        
        #define SDCARD_CS_PIN    10
        #define SDCARD_MOSI_PIN  7
        #define SDCARD_SCK_PIN   14

        
        void setupAudio();
        void setUpSD();
        void startRecording();
        void continueRecording();
        void stopRecording();


        float pulseTrain( float deltaG, unsigned long basalTime, unsigned long frecTime, float frequency);
        void rebote(float gain, float ecoTime, float pulseTime,float secondEcoTime, float frequency);

        extern int mode;
        extern bool state;
        extern bool pulseState;
#endif