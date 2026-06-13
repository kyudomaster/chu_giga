#include <Wire.h>
#include "VL53L0X.h"

/* ======================================
 * VL53L0X ToF Test - Single Sensor Test.
 * ======================================
 */

# define TCAADDR 0x70 // or 0x71 for the other ToF board.

VL53L0X sensor; // Declare there are 6 sensors. (Current board only has 1-5 not 0)

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();

  Serial.println("\nStarting ToF test...");
  
  sensor.setTimeout(500);

  if (!sensor.init())
  {
    Serial.println("Trying to start sensor");
    while(1);
  }
  else
  {
    Serial.println("Started succesfully!");
    sensor.startContinuous();
  }
}

void loop()
{
  uint16_t distance = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred())
  {
    Serial.println("Timeout");
  } 
  else 
  {
    Serial.print(distance);
    Serial.println(" mm");
  }

  Serial.println("---------------------");
  delay(200);
}