#include <Wire.h>
#include "VL53L0X.h"

/* =====================================
 * VL53L0X ToF Test - Full sensor board.
 * =====================================
 */

# define TCAADDR 0x70 // or 0x71 for the other ToF board.

VL53L0X sensor[6]; // Declare there are 6 sensors. (Current board only has 1-5 not 0)

// Select I2C channel on multiplexer
void tcaselect(uint8_t i)
{
  if (i > 7) return; // Only 8 channels available.

  Wire.beginTransmission(TCAADDR); // Select i2c device
  Wire.write(1 << i); 
  Wire.endTransmission();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();

  Serial.println("\nStarting ToF test...");

  // Initialize sensors on channels 1-5
  for (uint8_t ch = 0; ch < 6; ch++)
  {
    tcaselect(ch);
    delay(10); // Small delay after switching channel.

    Serial.print("Initializing sensor on channel ");
    Serial.println(ch);

    sensor[ch].setTimeout(500);

    if (!sensor[ch].init())
    {
      Serial.print("Failed to boot sensor on channel ");
      Serial.println(ch);
    }
    else
    {
      Serial.print("Sensor on channel ");
      Serial.println(ch);
      Serial.println(" ready!");
      sensor[ch].startContinuous();
    }
  }
  Serial.println("All sensors initialized.");
}

void loop()
{
  for (uint8_t ch = 0; ch < 6; ch++)
  {
    tcaselect(ch);

    uint16_t distance = sensor[ch].readRangeContinuousMillimeters();
    Serial.print("Sensor ");
    Serial.print(ch);
    Serial.print(": ");

    if (sensor[ch].timeoutOccurred())
    {
      Serial.println("Timeout");
    } 
    else 
    {
      Serial.print(distance);
      Serial.println(" mm");
    }
  }

  Serial.println("---------------------");
  delay(200);
}