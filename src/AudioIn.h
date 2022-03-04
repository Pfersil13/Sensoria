#ifndef AUDIOIN_H
    #define AUDIOIN_H
        
        #include <Arduino.h>
        #include <Audio.h>
        #include <Wire.h>
        #include <SPI.h>
        #include <SD.h>
        #include <SerialFlash.h>

        // Use these with the Teensy 4.1
        #define SDCARD_CS_PIN    BUILTIN_SDCARD
        #define SDCARD_MOSI_PIN  11  
        #define SDCARD_SCK_PIN   13  
        
        // Use these with the Teensy Audio Shield
        //#define SDCARD_CS_PIN    10
        //#define SDCARD_MOSI_PIN  7
        //#define SDCARD_SCK_PIN   14
        
        void setUpSD();
        void startRecording(char* name, char* noise);
        void continueRecording();
        void stopRecording(char* name);
        void sendBle(char* name);
        
        extern int mode;
        extern bool state;
        extern bool pulseState;
        
#endif