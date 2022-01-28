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


        void pulseTrain( float deltaG, float basalTime, float frecTime, float frequency);


        extern int mode;
        extern bool state;
#endif