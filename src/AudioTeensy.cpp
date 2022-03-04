#include <AudioTeensy.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "AudioIn.h"

// which input on the audio shield will be used?
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;


float gain = 0;
bool basal = 0;
bool state_2 = 0;
    

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=246.20001220703125,265.20001220703125
AudioSynthWaveformSine   sineLeft;          //xy=247.20001220703125,150.1999969482422
AudioSynthWaveformSine   sineRight;          //xy=249.20001220703125,191.1999969482422
AudioFilterStateVariable filter1;        //xy=406.20001220703125,250.1999969482422
AudioOutputI2S           i2s1;           //xy=426.20001220703125,156.1999969482422
AudioRecordQueue         queue1;         //xy=564.2000122070312,263.20001220703125
AudioRecordQueue         queue2;  //xy=565.2000122070312,309.79998779296875
AudioConnection          patchCord1(i2s2, 0, queue1, 0);
AudioConnection          patchCord2(i2s2, 1, queue2, 0);
AudioConnection          patchCord3(sineLeft, 0, i2s1, 0);
AudioConnection          patchCord4(sineRight, 0, i2s1, 1);
//AudioConnection          patchCord5(filter1, 2, queue1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=255.1999969482422,351.20001220703125
// GUItool: end automatically generated code

float pulseTrain( float deltaG, unsigned int basalTime, unsigned int frecTime, int frequency, char* fileName,char* noiseName, bool channel);
void rebote(float gain, float ecoTime, float pulseTime,float secondEcoTime, float frequency, bool channel, char* fileName);
void frecSweep(bool channel,int nFrec);

void createFile(bool channel, int frec , String *fileName, String *ambientNoise);


//Function for setting up audio library
void setupAudio(){

sgtl5000_1.enable();  //Enable codec
sgtl5000_1.dacVolume(1);  //Dac volume 1 -> 0 dB
sgtl5000_1.volume(0);     //Headphones volume 0
sgtl5000_1.inputSelect(myInput);  //Select audio input
//sgtl5000_1.micGain(45);       //Select mic gain (dB)
//sgtl5000_1.adcHighPassFilterDisable();    //Disabled because  it introduce a lot fo noise
sgtl5000_1.muteHeadphone();   
sgtl5000_1.muteLineout();
sineLeft.amplitude(0);
sineRight.amplitude(0);
sineLeft.frequency(0);
sineRight.frequency(0);
//filter1.frequency(100);
//filter1.resonance(0.5);
sgtl5000_1.lineInLevel(0,0);      //Line level 3.12 V p-p
//biquad1.setHighpass(1,200,0.7);
//biquad1.setHighpass(2,200,0.7);
//biquad1.setNotch(1,100,0.7);
//biquad1.setNotch(2,100,0.7);
//sgtl5000_1.lineOutLevel(13);
AudioMemory(200); 

}

//Esta función se encarga de generar el tren de pulsos para determinar la ganancia del oído

float pulseTrain( float deltaG, unsigned int basalTime, unsigned int frecTime, int frequency , char* fileName, char* noiseName,bool channel){
    
    unsigned long previousFrec = 0;        // will store last time frec was triggered
    unsigned long previousBasal = 0;      // will store last time frec was updated
    gain = -deltaG;
    sgtl5000_1.muteHeadphone();   //Mutea la salida
    startRecording(fileName, noiseName);             //Empieza a grabar          
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
        SineAmplitude(channel ,0);
        sgtl5000_1.muteHeadphone();   //Mutea la salida
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
          SineAmplitude(channel ,0);
          sgtl5000_1.muteHeadphone();   //Mutea la salida
          
          break;
        }
        sgtl5000_1.unmuteHeadphone();  //Desmutea la salida
        Serial.println(gain);
        SineAmplitude(channel ,1);
        previousFrec = millis();    //El tiempo del puslo empieza a contar ahora
        Serial.println("BasalEnd");
        start = 0;
        }

    continueRecording();  //Mientras sigue grabando

    }
    SineAmplitude(channel ,0);
    sgtl5000_1.muteHeadphone();   //Mutea la salida
  return gain;
}

//Esta función se encarga de generar el último pulso 

void rebote(float gain, float ecoTime, float pulseTime, float secondEcoTime, float frequency , bool channel,char* fileName){
  
  Serial.println("Im'in");
  Serial.println(millis());
  unsigned long firstTime = millis();
  unsigned long pulseStart = 0;
  unsigned long ecoStart = 0;
  sgtl5000_1.volume(gain);
  state_2 = 1;
  
  int eco = 1;
  while (state_2 == 1)
  {
    unsigned long currentTime = millis();

    if(currentTime - firstTime >= ecoTime && eco == 1){
      SineAmplitude(channel ,1);
      SineFrequency(channel, frequency);
      Serial.println(gain);
      sgtl5000_1.unmuteHeadphone();
      pulseStart = millis();
      eco = 0;
      Serial.println("Puslooo");
      Serial.println(millis());
      }
    if(currentTime - pulseStart >= pulseTime && eco == 0){
      SineAmplitude(channel ,0);
      sgtl5000_1.muteHeadphone();   //Mutea la salida
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


//

void frecSweep(bool channel,int nFrec){
  int frequency;
  char name[30], ambientNoiseName[30];
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
  String fileName,ambientName;
  createFile(channel, frequency , &fileName, &ambientName);  //Creates a filename based on channel y frecuency
  fileName.toCharArray(name,fileName.length()+1);
  ambientName.toCharArray(ambientNoiseName,ambientName.length()+1);
  Serial.println(name);
  Serial.println(ambientNoiseName);
  float audible = pulseTrain(Incremento_Gain, basalT, frecT, frequency ,name,ambientNoiseName, channel);
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


//Just a function in order to create the new names for files to be stored in sd. 

void createFile(bool channel, int frec , String *fileName, String *ambientNoise){
  String speaker = "0";
  if (channel  == 0){  
  speaker = "L";
    }else{ 
  speaker = "R";
    }
  *fileName = String("RECORD_") + speaker + String("_") + String(frec) + String(".RAW") ;
  *ambientNoise = String("AmbientNoise_") + speaker + String("_") + String(frec) + String(".RAW") ;

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
  bool channel = 1;
  frecSweep(channel,nFrec);
  //channel = 1;
  //frecSweep(channel,nFrec);

}


