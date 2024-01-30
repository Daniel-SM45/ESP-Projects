#include<DHT.h>
#include<LiquidCrystal_I2C.h>

#define _DHT_sensor_pin 4
#define _DHT_sensor_type DHT11


int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27,lcdColumns,lcdRows);


DHT dht_sensor(_DHT_sensor_pin, _DHT_sensor_type);
struct{
  
  double humi = dht_sensor.readHumidity();
  double temp_c = dht_sensor.readTemperature();
  double temp_f = dht_sensor.readTemperature(true);
  
}_DHT_config;

void setup() {
  
  Serial.begin(115200);
  
   lcd.init();
   lcd.backlight();
   
  _DHT_config.humi;
  _DHT_config.temp_c;
  _DHT_config.temp_f;

}

void loop() {

  lcd_display();

}
void lcd_display()
  {

    lcd.setCursor(0,0);
    lcd.print("Temp: " );
    lcd.print(_DHT_config.temp_c);
    lcd.print(" C");
    lcd.setCursor(0,0);

    lcd.setCursor(0,1);
    lcd.print("Humi: ");
    lcd.print(_DHT_config.humi);
    lcd.print("%");
    lcd.setCursor(0,1);
    
  }
