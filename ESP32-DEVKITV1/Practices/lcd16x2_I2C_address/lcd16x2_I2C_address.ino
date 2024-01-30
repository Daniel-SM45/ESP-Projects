#include<Wire.h>
#define _NEWLINE "\n"  

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}

void loop()
{

  byte error, address;
  int numDevices;

  Serial.println("Scanning...");
  numDevices = 0;

  for(address = 1; address < 127; address++){

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

 if(error == 0){

  Serial.println("I2C device found at address 0x");
     
    if(address < 16){
      Serial.print("0");
    }
    Serial.println(address, HEX);
    numDevices++;
 }
  else if (error == 4){

    Serial.print("Unknow error at address 0x");
    if (address < 16 ){
        Serial.print("0");
    }
    Serial.println(address, HEX);
  }
  if (numDevices == 0 ){
    Serial.println("No I2C Devices found");
  }
    else{
      Serial.println("Done!");
    }
    delay(5000);
  }


}
