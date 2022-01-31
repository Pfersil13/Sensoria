#ifndef MIC_H
    #define MIC_H
        
        #include <Arduino.h>
       
        // Use these with the Teensy 4.1
        #define SDCARD_CS_PIN    BUILTIN_SDCARD
        #define SDCARD_MOSI_PIN  11  
        #define SDCARD_SCK_PIN   13  
        
        #define Frec_1  1000
        #define Frec_2  2000
        #define Frec_3  3000
        #define Frec_4  4000
        #define Frec_5  5000
        #define Frec_6  6000
        #define Frec_7  7000
        #define Frec_8  8000
        #define Frec_9  9000
        #define Frec_10 10000

        #define basalT   1000
        #define frecT    1000

        #define ecoT 3000
        #define pulseT 500
        #define secondEcoT   5000

        // Use these with the Teensy Audio Shield
        //#define SDCARD_CS_PIN    10
        //#define SDCARD_MOSI_PIN  7
        //#define SDCARD_SCK_PIN   14

        
        void setupAudio();
        void setUpSD();
        void startRecording(char* name);
        void continueRecording();
        void stopRecording();


        float pulseTrain( float deltaG, unsigned int basalTime, unsigned int frecTime, int frequency, char* fileName);
        void rebote(float gain, float ecoTime, float pulseTime,float secondEcoTime, float frequency);
        void frecSweep(bool speaker,int nFrec);

        String createFile(bool speaker, int frec);

        extern int mode;
        extern bool state;
        extern bool pulseState;
#endif