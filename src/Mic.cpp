#include <Mic.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// The file where data is recorded
File frec;

// which input on the audio shield will be used?
//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

unsigned long previousFrec = 0;        // will store last time LED was updated
unsigned long previousBasal = 0;
  float gain = 0;
bool basal = 0;
        // constants won't change:
    
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=247.20001220703125,150.1999969482422
AudioInputI2S            i2s2;           //xy=249.1999969482422,245.1999969482422
AudioRecordQueue         queue1;         //xy=414.1999969482422,243.1999969482422
AudioOutputI2S           i2s1;           //xy=426.20001220703125,156.1999969482422
AudioConnection          patchCord1(sine1, 0, i2s1, 0);
//AudioConnection          patchCord2(sine1, 0, i2s1, 1);
AudioConnection          patchCord3(i2s2, 1, queue1, 0);
// GUItool: end automatically generated codeated code
AudioControlSGTL5000     sgtl5000_1; 
// GUItool: end automatically generated code

void setupAudio(){

sgtl5000_1.enable();
sgtl5000_1.dacVolume(1);
sgtl5000_1.volume(0); 
sgtl5000_1.inputSelect(myInput);
sgtl5000_1.micGain(5);
sgtl5000_1.adcHighPassFilterDisable();
sgtl5000_1.unmuteHeadphone();
sgtl5000_1.muteLineout();
   //sine1.amplitude(1);
   //sine1.frequency(1000);
//sgtl5000_1.lineOutLevel(13);
AudioMemory(120); 

}

void setUpSD(){
    SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here if no SD card, but print a message
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }}
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
  sgtl5000_1.muteHeadphone();
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
  state = 0;
}





void pulseTrain( float deltaG, float basalTime, float frecTime, float frequency){
    startRecording();
    sine1.amplitude(1);
    sine1.frequency(frequency);
    sgtl5000_1.volume(gain);
    sgtl5000_1.unmuteHeadphone();
    state = 1;
    while (state == 1)
    {         
unsigned long currentTime = millis(); 
if(currentTime - previousFrec >= frecTime && basal == 0){
//continueRecording();
gain = gain + deltaG;
sgtl5000_1.volume(gain);
previousBasal = currentTime;
sgtl5000_1.muteHeadphone();
Serial.println("FrecEnd");
Serial.println(gain);
basal = 1;
}
if(currentTime - previousBasal >= basalTime && basal == 1){
//continueRecording();
sgtl5000_1.unmuteHeadphone();
previousFrec = currentTime;
Serial.println("BasalEnd");
basal = 0;
}
continueRecording();
    

}
}