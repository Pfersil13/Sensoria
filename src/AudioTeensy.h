#ifndef AUDIOTEENSY_H
    #define AUDIOTEENSY_H
        
        #include <Arduino.h>
       
        // Use these with the Teensy 4.1
        #define SDCARD_CS_PIN    BUILTIN_SDCARD
        #define SDCARD_MOSI_PIN  11  
        #define SDCARD_SCK_PIN   13  
        
        //This frecuecuncis are in order to variate frecuency in a non linear way
        //Select until with frecuency (nFrec) you want to test. 
        #define Frec_1  250
        #define Frec_2  500
        #define Frec_3  1000
        #define Frec_4  2000
        #define Frec_5  4000
        #define Frec_6  7000
        #define Frec_7  10000
        #define Frec_8  13000
        #define Frec_9  15000
        #define Frec_10 17000

        #define Incremento_Gain 0.05
        #define basalT   500       //Define basal lenght
        #define frecT    200       //Define pulse lenght

        #define ecoT 3000       //Define first eco lenght
        #define pulseT 50      //Define final pulse lenght
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