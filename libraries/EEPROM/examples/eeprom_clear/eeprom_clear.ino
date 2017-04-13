/*
 * EEPROM Clear
 *
 * Sets all of the bytes of the EEPROM to 0.
 * This example code is in the public domain.

 */
#include <EEPROM.h>

void setup()
{
  pinMode(13, OUTPUT);
  // write a 0 to all 512 bytes of the EEPROM, this method might take a couple of seconds to complete
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
    
  /* 
  //This is another method to write a 0 to all 512 addresses
  //It is a must to erase the address before writing new data into it!
  //The value in address is set to 0xFFFFFFFF by default after erase.
  EEPROM.eraseAddressBlock(0); //clear address 0 - 127
  EEPROM.eraseAddressBlock(1); //clear address 128 - 255
  EEPROM.eraseAddressBlock(2); //clear address 256 - 383
  EEPROM.eraseAddressBlock(3); //clear address 384 - 511
  
  for (int i = 0; i < 512; i++)
    EEPROM.fastWrite(i, 0);
  */
  
  // turn the LED on when we're done
  digitalWrite(13, HIGH);
}

void loop()
{
}
