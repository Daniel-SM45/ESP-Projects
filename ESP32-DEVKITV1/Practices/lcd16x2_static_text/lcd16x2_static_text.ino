#include <LiquidCrystal_I2C.h>

int lcdColumns = {16};
int lcdRows = {2};

LiquidCrystal_I2C lcd( 0x27, lcdColumns, lcdRows);

void setup() 
{

lcd.init();      //Inicializa o Display LCD
lcd.backlight(); // Ativa a luz de fundo do LCD

}

void loop()
{

  lcd.setCursor(0,1);
  dotAnim();
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Hello ESP32! ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  
}
void dotAnim(){
  for(int idx = 0; idx < 3; idx++){
      lcd.print(".");
      delay(500);
  }
}
