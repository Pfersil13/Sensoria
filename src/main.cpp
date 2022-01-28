#include <Arduino.h>

#include <Mic.h>

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording, 2=playing
const int Button_1 = 32;
bool state = 0;

void ISR(){

stopRecording();
delay(5000);
}
void setup() {
  setupAudio();
  setUpSD();
  pinMode(Button_1, INPUT_PULLUP);
Serial.begin(115200);
attachInterrupt(digitalPinToInterrupt(Button_1), ISR, RISING);
mode = 0;
pulseTrain( 0.05, 10000, 10000, 1000);
}

void loop() {
Serial.println(".");

}

