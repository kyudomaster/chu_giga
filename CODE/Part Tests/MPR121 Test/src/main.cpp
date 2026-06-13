#include <Wire.h>
#include "MPR121.h"

/* ===================
 * MPR121 Test - Multiple boards. (all 3)
 * =================== 
 */

MPR121 sensors[] = { MPR121(0x5A), MPR121(0x5B), MPR121(0x5C) };

void setup() 
{
  Serial.begin(115200);
  while (!Serial) {}

  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();

  for (int s = 0; s < 3; s++)
  {
    if (!sensors[s].begin()) 
    {
      Serial.print("MPR121 ");
      Serial.print(s + 1);
      Serial.println(" not found");
      while (1);
    }
  }

  Serial.println("MPR121 all ready");
}

void loop()
{
  for (int s = 0; s < 3; s++)
  {
    uint16_t state = sensors[s].touched();

    for (int i = 0; i < 12; i++)
    {
      if (state & (1 << i))
      {
        Serial.print("Touched key ");
        Serial.println(i + 12*s);
      }
    }
  }
  delay(10);
  return;

  // // debugging info, what
  // Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x");
  // Serial.println(sensors[0].touched(), HEX);
  // Serial.println(sensors[1].touched(), HEX);
  // Serial.println(sensors[2].touched(), HEX);
  // Serial.print("Filt: ");
  // for (uint8_t i = 0; i < 12; i++) {
  //   Serial.print(sensors[0].filterData(i));
  //   Serial.print("\t");
  // }
  // Serial.println();
  // Serial.print("Base: ");
  // for (uint8_t i = 0; i < 12; i++) {
  //   Serial.print(sensors[0].baseData(i));
  //   Serial.print("\t");
  // }
  // Serial.println();

  // delay(100);
}
