#include <AudioTeensy.h>
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


  float gain = 0;
  bool basal = 0;
  bool state_2 = 0;
        // constants won't change:
    
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sineLeft;          //xy=247.20001220703125,150.1999969482422
AudioSynthWaveformSine   sineRight;          //xy=248.1999969482422,191.1999969482422
AudioInputI2S            i2s2;           //xy=249.1999969482422,245.1999969482422
AudioRecordQueue         queue1;         //xy=414.1999969482422,243.1999969482422
AudioOutputI2S           i2s1;           //xy=426.20001220703125,156.1999969482422
AudioConnection          patchCord1(sineLeft, 0, i2s1, 0);
AudioConnection          patchCord2(sineRight, 0, i2s1, 1);
AudioConnection          patchCord3(i2s2, 1, queue1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=288.1999969482422,384.1999969482422
// GUItool: end automatically generated code

void setupAudio(){

sgtl5000_1.enable();
sgtl5000_1.dacVolume(1);
sgtl5000_1.volume(0); 
sgtl5000_1.inputSelect(myInput);
sgtl5000_1.micGain(0);
sgtl5000_1.adcHighPassFilterDisable();
sgtl5000_1.muteHeadphone();
sgtl5000_1.muteLineout();
sineLeft.amplitude(0);
sineRight.amplitude(0);
sineLeft.frequency(0);
sineRight.frequency(0);
//sgtl5000_1.lineOutLevel(13);
AudioMemory(200); 

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



void startRecording(char* name) {
  Serial.println("startRecording");
  if (SD.exists(name)) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove(name);
  }
  frec = SD.open(name, FILE_WRITE);
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
}

void stopRecording(char* name) {
  sgtl5000_1.muteHeadphone();
  Serial.println("stopRecording");
  queue1.end();
  if (mode == 1) {
    while (queue1.available() > 0) {
      frec.write((byte*)queue1.readBuffer(), 256);
      queue1.freeBuffer();
    }
    frec.close();
    
    // Abrir fichero y mostrar el resultado
  frec = SD.open(name); 
  if(frec){
    Serial.println(frec.size());
    while (frec.available())
   {
   Serial6.write(frec.read());
   //Serial.println(frec.position());
    }
  }
  else 
  {
    Serial.println(F("Error al abrir el archivo"));
  }
  frec.close();
  mode = -2;
  state = 0;
}}



//Esta función se encarga de generar el tren de pulsos para determinar la ganancia del oído

float pulseTrain( float deltaG, unsigned int basalTime, unsigned int frecTime, int frequency , char* fileName, bool channel){
    
    unsigned long previousFrec = 0;        // will store last time LED was updated
    unsigned long previousBasal = 0;
    gain = 0;
    sgtl5000_1.muteHeadphone();   //Mutea la salida
    startRecording(fileName);             //Empieza a grabar
    //sineLeft.amplitude(0);           //Establece la amplitud de la senoidal 
    SineAmplitude(channel , 0);
    SineFrequency(channel,frequency);
    //sineLeft.frequency(frequency);   //Establece la frecuencia de la senoidal 
    sgtl5000_1.volume(0);      //Declara la ganacia de la salida
    

    //previousFrec = millis();
    //sgtl5000_1.unmuteHeadphone(); //Mutea la salida
    state = 1;
    basal = 1;
    bool start = 1;
    while (state == 1)            
    {         
//unsigned long currentTime = millis();

if(millis() - previousFrec >= frecTime && basal == 0){   //Si ha pasado el tiempo del pulso 
Serial.println(millis());
sgtl5000_1.muteHeadphone();   //Mutea la salida
SineAmplitude(channel ,0);
previousBasal = millis();  //El tiempo basal empieza a contar ahora
Serial.println("FrecEnd");
//Serial.println(gain);
Serial.println(millis());
basal = 1;
}
 
if(start == 1 || (millis() - previousBasal >= basalTime && basal == 1) ){  //Si ha pasado el tiempo basal  
  basal = 0;
  gain = gain + deltaG;         //Incrementa la gananacia
  sgtl5000_1.volume(gain);      //Declara la nueva ganancia
  if(gain >= 0.9){
    stopRecording(fileName);
    break;
  }
  sgtl5000_1.unmuteHeadphone();  //Desmutea la salida
  SineAmplitude(channel ,1);
  previousFrec = millis();    //El tiempo del puslo empieza a contar ahora
  
  Serial.println("BasalEnd");
  Serial.println(millis());
  start = 0;
}
continueRecording();  //Mientras sigue grabando
}
sgtl5000_1.muteHeadphone();
SineAmplitude(channel,0);
return gain;
}

void rebote(float gain, float ecoTime, float pulseTime, float secondEcoTime, float frequency , bool channel,char* fileName){
  Serial.println("Im'in");
  Serial.println(millis());
unsigned long firstTime = millis();
unsigned long pulseStart = 0;
unsigned long ecoStart = 0;
state_2 = 1;
int eco = 1;
while (state_2 == 1)
{
 
  unsigned long currentTime = millis();

  if(currentTime - firstTime >= ecoTime && eco == 1){
    SineAmplitude(channel ,1);
    SineFrequency(channel, frequency);
    sgtl5000_1.volume(gain);
    sgtl5000_1.unmuteHeadphone();
    pulseStart = millis();
    eco = 0;
      Serial.println("Puslooo");
      Serial.println(millis());
  }
  if(currentTime - pulseStart >= pulseTime && eco == 0){
    SineAmplitude(channel ,0);
    sgtl5000_1.muteHeadphone();
    ecoStart = millis();
    eco = 2;
          Serial.println("adios, pulso");
          Serial.println(millis());

  }
   continueRecording();
  if(currentTime - ecoStart >= secondEcoTime && eco == 2){
    state_2 = 0;
    stopRecording(fileName);
    pulseState = 1;
  }
}



}

void frecSweep(bool channel,int nFrec){
  int frequency;
  char name[30];
  for(int i = 0; i <= nFrec -1; i++){

    switch (i)
    {
    case 0:
      frequency = Frec_1;
      break;
    case 1:
      frequency = Frec_2;
      break;
    case 2:
      frequency = Frec_3;
      break;
    case 3:
      frequency = Frec_4;
      break;
    case 4:
      frequency = Frec_5;
      break;
    case 5:
      frequency = Frec_6;
      break;
    case 6:
      frequency = Frec_7;
      break;
    case 7:
      frequency = Frec_8;
      break;
    case 8:
      frequency = Frec_9;
      break;
    case 9:
      frequency = Frec_10;
      break;
    default:
    frequency = 0;
      break;
    }
  Serial.println(frequency);
  String fileName = createFile(channel, frequency);
  fileName.toCharArray(name,fileName.length()+1);
  Serial.println(name);
  float audible = pulseTrain( 0.1, basalT, frecT, frequency ,name, channel);
  if(audible < 0.9){
  Serial.print("Gain: ");
  Serial.println(audible);
  rebote(audible, ecoT, pulseT, secondEcoT, frequency, channel, name );
  }else{
    Serial.println("No response for this frequency" );
  }
  }
  Serial.println("The END");
}

String createFile(bool channel, int frec){
  String speaker = "0";
  if (channel  == 0)    
  speaker = "L";
    else 
  speaker = "R";

  String mergedTopic = String("RECORD_") + speaker + String("_") + String(frec) + String(".RAW") ;
  
  return mergedTopic;
}


void SineAmplitude(bool channel, float state ){
if(channel == 1){
  sineLeft.amplitude(state);
}else{
  sineRight.amplitude(state);
}}



void SineFrequency(bool channel, int frec ){
if(channel == 1){
  sineLeft.frequency(frec);

}else{
  sineRight.frequency(frec);
 
}}


void startCycle(int nFrec){
  bool channel = 0;
  frecSweep(channel,nFrec);
  channel = 1;
  frecSweep(channel,nFrec);

}
