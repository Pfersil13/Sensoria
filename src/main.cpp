#include <Arduino.h>
#include <Mic.h>

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
void setup() {
  setupAudio();
  setUpSD();
  pinMode(Button_1, INPUT_PULLUP);
  pinMode(Button_2, INPUT_PULLUP);
Serial.begin(115200);
attachInterrupt(digitalPinToInterrupt(Button_1), ISR, RISING);
attachInterrupt(digitalPinToInterrupt(Button_2), ISR_2, RISING);
//float audible = pulseTrain( 0.1, 1000, 1000, 1000);


}

void loop() {
if(pulseState == 0){
float audible = pulseTrain( 0.1, 1000, 1000, 1000);
Serial.print("Gain: ");
Serial.println(audible);
rebote(audible, 3000, 500, 5000, 1000);
}
}

