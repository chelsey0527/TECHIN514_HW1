#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define VIBRATOR_PIN 6 // Change this to the pin connected to your vibrator

Adafruit_BME280 bme;
Adafruit_MPU6050 mpu;

void triggerVibrator(); // Function prototype

void setup()
{
  Serial.begin(115200);

  pinMode(VIBRATOR_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
      ;
  }

  Serial.println("Ready!");
}

void loop()
{
  // sensors_event_t event;
  bme.takeForcedMeasurement();

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.println(temperature);
  display.print("Humidity: ");
  display.println(humidity);
  display.print("Accel X: ");
  display.println(a.acceleration.x);
  display.print("Accel Y: ");
  display.println(a.acceleration.y);
  display.print("Accel Z: ");
  display.println(a.acceleration.z);
  display.display();
  Serial.println('hello');

  // Check for motion
  if (abs(a.acceleration.x) > 2 || abs(a.acceleration.y) > 2 || abs(a.acceleration.z) > 2)
  {
    triggerVibrator();
  }

  delay(1000); // Adjust the delay as needed
}

void triggerVibrator()
{
  digitalWrite(VIBRATOR_PIN, HIGH);
  delay(1000); // Adjust the vibration duration as needed
  digitalWrite(VIBRATOR_PIN, LOW);
}