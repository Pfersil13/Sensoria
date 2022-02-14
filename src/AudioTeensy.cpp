#include <AudioTeensy.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "AudioIn.h"

// which input on the audio shield will be used?
//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;


float gain = 0;
bool basal = 0;
bool state_2 = 0;
    
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


float pulseTrain( float deltaG, unsigned int basalTime, unsigned int frecTime, int frequency, char* fileName, bool channel);
void rebote(float gain, float ecoTime, float pulseTime,float secondEcoTime, float frequency, bool channel, char* fileName);
void frecSweep(bool channel,int nFrec);

String createFile(bool channel, int frec);


//Function for setting up audio library
void setupAudio(){

sgtl5000_1.enable();  //Enable codec
sgtl5000_1.dacVolume(1);  //Dac volume 1 -> 0 dB
sgtl5000_1.volume(0);     //Headphones volume 0
sgtl5000_1.inputSelect(myInput);  //Select mic input
sgtl5000_1.micGain(5);       //Select mic gain (dB)
sgtl5000_1.adcHighPassFilterDisable();    //Disabled because  it introduce a lot fo noise
sgtl5000_1.muteHeadphone();   
sgtl5000_1.muteLineout();
sineLeft.amplitude(0);
sineRight.amplitude(0);
sineLeft.frequency(0);
sineRight.frequency(0);
//sgtl5000_1.lineOutLevel(13);
AudioMemory(200); 

}

//Esta función se encarga de generar el tren de pulsos para determinar la ganancia del oído

float pulseTrain( float deltaG, unsigned int basalTime, unsigned int frecTime, int frequency , char* fileName, bool channel){
    
    unsigned long previousFrec = 0;        // will store last time frec was triggered
    unsigned long previousBasal = 0;      // will store last time frec was updated
    gain = 0;
    sgtl5000_1.muteHeadphone();   //Mutea la salida
    startRecording(fileName);             //Empieza a grabar          
    SineAmplitude(channel , 0);     //Establece la amplitud de la senoidal 
    SineFrequency(channel,frequency);   //Establece la frecuencia de la senoidal 
    sgtl5000_1.volume(0);      //Declara la ganacia de la salida
    
    state = 1;
    basal = 1;
    bool start = 1;
    while (state == 1)            
    {         
    //unsigned long currentTime = millis();
      if(millis() - previousFrec >= frecTime && basal == 0){   //Si ha pasado el tiempo del pulso 
        sgtl5000_1.muteHeadphone();   //Mutea la salida
        SineAmplitude(channel ,0);
        previousBasal = millis();  //El tiempo basal empieza a contar ahora
        Serial.println("FrecEnd");
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
  //channel = 1;
  //frecSweep(channel,nFrec);

}


