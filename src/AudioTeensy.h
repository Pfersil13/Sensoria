#ifndef AUDIOTEENSY_H
    #define AUDIOTEENSY_H
        
        #include <Arduino.h>
       
        // Use these with the Teensy 4.1
        #define SDCARD_CS_PIN    BUILTIN_SDCARD
        #define SDCARD_MOSI_PIN  11  
        #define SDCARD_SCK_PIN   13  
        
        //This frecuecuncis are in order to variate frecuency in a non linear way
        //Select until with frecuency (nFrec) you want to test. 
        #define Frec_1  1000
        #define Frec_2  5000
        #define Frec_3  10000
        #define Frec_4  15000
        #define Frec_5  5000
        #define Frec_6  6000
        #define Frec_7  7000
        #define Frec_8  8000
        #define Frec_9  9000
        #define Frec_10 10000

        #define basalT   1000       //Define basal lenght
        #define frecT    1000       //Define pulse lenght

        #define ecoT 3000       //Define first eco lenght
        #define pulseT 500      //Define final pulse lenght
        #define secondEcoT   5000   //Define second eco lenght

        // Use these with the Teensy Audio Shield
        //#define SDCARD_CS_PIN    10
        //#define SDCARD_MOSI_PIN  7
        //#define SDCARD_SCK_PIN   14

        
        void setupAudio();
        void SineAmplitude(bool channel, float state );
        void SineFrequency(bool channel, int frec);
        void startCycle(int nFrec);

        extern int mode;
        extern bool state;
        extern bool pulseState;
#endif