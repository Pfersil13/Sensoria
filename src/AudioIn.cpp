#include "AudioIn.h"

// The file where data is recorded
File frec;
File noiseAudio;
// GUItool: begin automatically generated code
extern AudioSynthWaveformSine   sineLeft;          //xy=247.20001220703125,150.1999969482422
extern AudioSynthWaveformSine   sineRight;          //xy=248.1999969482422,191.1999969482422
extern AudioInputI2S            i2s2;           //xy=249.1999969482422,245.1999969482422
extern AudioRecordQueue         queue1;         //xy=414.1999969482422,243.1999969482422
extern AudioRecordQueue         queue2;         //xy=414.1999969482422,243.1999969482422
extern AudioOutputI2S           i2s1;           //xy=426.20001220703125,156.1999969482422
extern AudioControlSGTL5000     sgtl5000_1;     //xy=288.1999969482422,384.1999969482422
// GUItool: end automatically generated code




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

void startRecording(char* name, char* noise) {
  Serial.println("startRecording");
  if (SD.exists(name)) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove(name);
  }
  if (SD.exists(noise)) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove(noise);
  }
  frec = SD.open(name, FILE_WRITE); //Create file called "name"
  if (frec) {
    queue1.begin();     //Start recording
    mode = 1;           //Change to mode: recording
  }
  noiseAudio = SD.open(noise, FILE_WRITE); //Create file called "noise"
  if (noiseAudio) {
    queue2.begin();     //Start recording noise
    mode = 1;           //Change to mode: recording
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
    //Serial6.write(buffer,512);
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
  if(queue2.available() >= 2){
    byte buffer[512];
    memcpy(buffer, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    memcpy(buffer+256, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    noiseAudio.write(buffer, 512);
  }
}

void stopRecording(char* name) {
  sgtl5000_1.muteHeadphone();
  Serial.println("stopRecording");
  queue1.end();
  queue2.end();
  if (mode == 1) {
    while (queue1.available() > 0) {
      frec.write((byte*)queue1.readBuffer(), 256);
      queue1.freeBuffer();
    }
    while (queue2.available() > 0) {
      noiseAudio.write((byte*)queue2.readBuffer(), 256);
      queue2.freeBuffer();
      //queue2.clear();  
      //Serial.println("Here");
    }
    frec.close();
    noiseAudio.close();

  mode = -2;
  state = 0;
}}


void sendBle(char* name){

    frec = SD.open(name); 
  if(frec){
    Serial.println(frec.size());
    while (frec.available())
   {
   uint8_t buffer[512];
   
   frec.read(buffer, sizeof(buffer));
   //elapsedMicros usec = 0;
   Serial6.write(buffer,sizeof(buffer));
   //Serial.println(usec);

    }
  }
  else 
  {
    Serial.println(F("Error al abrir el archivo"));
  }
  frec.close();
  Serial.println("End");
}