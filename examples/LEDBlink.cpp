#include <Arduino.h>
#include "FXL6408.hpp"

FXL6408 EGpio;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while(!Serial){  }
  EGpio.init();
  EGpio.gpio4.mode = FXL6408_GPIO_OUTPUT;
}


void loop() {
  // put your main code here, to run repeatedly:

  if(EGpio.pinWrite(FXL6408_GPIO_4, FXL6408_GPIO_LOW) != FXL6408_NOK){
    Serial.println("LOW");
    delay(1000);
  }

  if(EGpio.pinWrite(FXL6408_GPIO_4, FXL6408_GPIO_HIGH) != FXL6408_NOK){
    Serial.println("HIGH");
    delay(1000);
  }
}
