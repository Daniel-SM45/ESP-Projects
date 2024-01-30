
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHT_SENSOR_PIN  4 // ESP32 pin GPIO21 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  Serial.begin(115200);
  dht_sensor.begin(); // initialize the DHT sensor
  lcd.init();
  lcd.backlight();
}

void loop() {

  double humi  = dht_sensor.readHumidity();
  double tempC = dht_sensor.readTemperature();
  double tempF = dht_sensor.readTemperature(true);

  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    
    Serial.print("Humidade: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperatura: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");

    ///LCD

    lcd.setCursor(0,0);
    lcd.print("Temp: " );
    lcd.print(tempC);
    lcd.print(" C");
    lcd.setCursor(0,0);

    lcd.setCursor(0,1);
    lcd.print("Humi: ");
    lcd.print(humi);
    lcd.print("%");
    lcd.setCursor(0,1);
   
  }

  // wait a 2 seconds between readings
  delay(2000);
}
