#include <Arduino.h>
#include <AudioTeensy.h>

// Remember which mode we're doing
int mode = -2;  // 0=stopped, 1=recording, 2=playing
const int Button_1 = 32;
const int Button_2 = 33;
bool state = 0;
bool pulseState = 1;
void ISR(){
  pulseState = 0;
}
void ISR_2(){
  state = 0;
}
int a = 0;
void setup() {
  setupAudio();
  setUpSD();
  pinMode(Button_1, INPUT_PULLUP);
  pinMode(Button_2, INPUT_PULLUP);
Serial.begin(115200);
Serial6.begin(115200, SERIAL_8N1 );
attachInterrupt(digitalPinToInterrupt(Button_1), ISR, RISING);
attachInterrupt(digitalPinToInterrupt(Button_2), ISR_2, RISING);
//SineAmplitude(1,1);
//SineFrequency(1,1000);

//float audible = pulseTrain( 0.1, 1000, 1000, 1000);
//char name[30] = "Prueba_Bluetooth.RAW";
//sendBle(name);
}

void loop() {
if(pulseState == 0){
startCycle(1);
}
/*a++;
if (Serial6.available() > 0) {

    String inByte = Serial6.readString(120);
    Serial.println(inByte);
  }*/
/*if(a > 10000000)
{
  a = 0;
  int alfa = 3;
  Serial6.println(alfa);
  Serial.println("Hi");

}*/
}


