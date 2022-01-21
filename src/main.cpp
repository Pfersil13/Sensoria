#include <Arduino.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=247.20001220703125,150.1999969482422
AudioInputI2S            i2s2;           //xy=249.1999969482422,245.1999969482422
AudioRecordQueue         queue1;         //xy=414.1999969482422,243.1999969482422
AudioOutputI2S           i2s1;           //xy=426.20001220703125,156.1999969482422
AudioConnection          patchCord1(sine1, 0, i2s1, 0);
AudioConnection          patchCord2(sine1, 0, i2s1, 1);
AudioConnection          patchCord3(i2s2, 1, queue1, 0);
// GUItool: end automatically generated codeated code



AudioControlSGTL5000     sgtl5000_1; 
// GUItool: end automatically generated code
void pulseTrain(float Vmin, float Vmax, float deltaV, float basalTime, float frecTime, float frec);
void startRecording();
void continueRecording();
void stopRecording();
// which input on the audio shield will be used?
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording, 2=playing

// The file where data is recorded
File frec;


void setup() {
sgtl5000_1.enable();
sgtl5000_1.volume(0); 
sgtl5000_1.inputSelect(myInput);
//sine1.amplitude(1);
//sine1.frequency(20000);
AudioMemory(60); 
Serial.begin(115200);

SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here if no SD card, but print a message
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }}

mode = 0;
delay(1000);
sine1.amplitude(1);
sine1.frequency(15000);
}

void loop() {

sine1.amplitude(1);
sine1.frequency(15000);

if (mode == 0){
  startRecording();
}
if (mode == 1) {
    continueRecording();
  }
  if (millis() > 35000){
Serial.print("Done");
stopRecording();
  }

}


void startRecording() {
  Serial.println("startRecording");
  if (SD.exists("RECORD.RAW")) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove("RECORD.RAW");
  }
  frec = SD.open("RECORD.RAW", FILE_WRITE);
  if (frec) {
    queue1.begin();
    mode = 1;
  }
}

void continueRecording() {
  if (queue1.available() >= 2) {
    byte buffer[512];
    // Fetch 2 blocks from the audio library and copy
    // into a 512 byte buffer.  The Arduino SD library
    // is most efficient when full 512 byte sector size
    // writes are used.
    memcpy(buffer, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    memcpy(buffer+256, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    // write all 512 bytes to the SD card
    //elapsedMicros usec = 0;
    frec.write(buffer, 512);
    // Uncomment these lines to see how long SD writes
    // are taking.  A pair of audio blocks arrives every
    // 5802 microseconds, so hopefully most of the writes
    // take well under 5802 us.  Some will take more, as
    // the SD library also must write to the FAT tables
    // and the SD card controller manages media erase and
    // wear leveling.  The queue1 object can buffer
    // approximately 301700 us of audio, to allow time
    // for occasional high SD card latency, as long as
    // the average write time is under 5802 us.
    //Serial.print("SD write, us=");
    //Serial.println(usec);
  }
}

void stopRecording() {
  Serial.println("stopRecording");
  queue1.end();
  if (mode == 1) {
    while (queue1.available() > 0) {
      frec.write((byte*)queue1.readBuffer(), 256);
      queue1.freeBuffer();
    }
    frec.close();
  }
  mode = -2;
}

