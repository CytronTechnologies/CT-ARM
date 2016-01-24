/*
 * EEPROM FastWrite
 * This example is suitable for writing large chunk of data at once quickly
 * to the storage.
 *
 * The current data in the address must be erased first before being replaced by new data
 * 1 block contains 128 addresses, there is a total of 7 blocks, 1 address can store up to 4 bytes
 *
 *	1st block  Address 0 - 127
 *	2nd block  Address 128 - 255
 *	3rd block  Address 256 - 383
 *	4th block  Address 384 - 511
 *  ...
 *  7th block  Address 768 - 895
 */
#include <EEPROM.h>

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
int addr = 0;
int val = 0;

void setup()
{
  pinMode(13, OUTPUT);
  
  // Erase the data from first 4 blocks for writing new 512 data 
  EEPROM.eraseAddressBlock(0);
  EEPROM.eraseAddressBlock(1);
  EEPROM.eraseAddressBlock(2);
  EEPROM.eraseAddressBlock(3); 
}

void loop()
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  EEPROM.fastWrite(addr, val);
  
  // advance to the next address.  there are 512 bytes in 
  // the EEPROM, so go back to 0 when we hit 512.
  addr = addr + 1;
  val = addr;
  
  if (addr == 512)
  {
    //LED will light up once the process of writing all 512
    //data is finished
    digitalWrite(13, HIGH);
    while(1);
  }

}
