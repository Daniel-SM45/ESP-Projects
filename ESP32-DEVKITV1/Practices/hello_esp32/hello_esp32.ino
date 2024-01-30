//#define LED 2

struct sysObj{

unsigned short blink = 4;
short pinLED = {2};
char name [40] = {"Hello ESP32!"};

}myObjs;

sysObj introName;

void setup() {

  Serial.begin(115200);
  pinMode(myObjs.pinLED, OUTPUT);

}

void loop() {

    Serial.println(introName.name);
    delay(500);

    dot_anim ();

  for(int index = 0; index < myObjs.blink; index++ ){

    digitalWrite(myObjs.pinLED, 1);
    delay(200);
    digitalWrite(myObjs.pinLED, 0);
    delay(100);

  }
    /*digitalWrite(LED, 1);
    delay(200);
    digitalWrite(LED, 0);
    delay(200);*/

  
}
void  dot_anim (){

  for(int idx = 0; idx < 3 ; idx++ ){
      Serial.print(".");
      delay(500);
    }
}
