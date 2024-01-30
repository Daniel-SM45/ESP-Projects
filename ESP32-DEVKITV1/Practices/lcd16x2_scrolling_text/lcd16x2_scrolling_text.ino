#include<LiquidCrystal_I2C.h>

short lcdColumns = {16};
short lcdRows = {2};

LiquidCrystal_I2C lcd(0x27,lcdColumns, lcdRows);

String staticMessage = {"Ho Ho Ho! "};
String scrollMessage = {"Um otimo Natal pra vcs galera da NET"};

byte customChar[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b00100,
  0b00100
};
void setup()
{

 lcd.init();
 lcd.backlight();
 lcd.createChar(0, customChar);
}

void loop() {
  
  staticTextFunc();
  scrollTexFunc(1,scrollMessage, 250, lcdColumns);
  lcd.clear();
}

void scrollTexFunc(int row, String message, int delayTime, int lcdColumns){
    
    for(int idx = 0; idx < lcdColumns; idx++){

        message = " " + message;
    }
    message = message + " ";

    for (int pos = 0; pos < message.length(); pos++){
        lcd.setCursor(0, row);
        lcd.print(message.substring(pos,pos + lcdColumns));
        delay(delayTime);
    }
}
void staticTextFunc(){
 
  lcd.setCursor(0,0);
  dotAnim();
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(staticMessage);
  lcd.write(0);
  lcd.write(0);
  lcd.write(0);
  delay(1500);
  
}
void dotAnim(){
  for(int idx = 0; idx < 3; idx++){
      lcd.print(".");
      delay(500);
  }
}
