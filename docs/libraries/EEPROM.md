# EEPROM - Emulated

CT-ARM does not have actual EEPROM, instead it has data flash memory which size is up to 4KB. Here we are using EEPROM-like library to gain read/write access to the data flash. There are total of 1024 EEPROM addresses, each address is able to store up to 4 bytes.

## read()

**Description**

Reads value (in form of 4 bytes) from the EEPROM. Locations that have never been written to have the value of 0xFFFF.

**Syntax**

`EEPROM.read(address)`

**Parameters**

 - address: the location to read from, starting from 0 (int) to 1023

**Returns**

the value stored in that location (4 bytes)

**Example**

``` c++
#include <EEPROM.h>

int a = 0;
int value;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  value = EEPROM.read(a);

  Serial.print(a);
  Serial.print("\t");
  Serial.print(value);
  Serial.println();

  a = a + 1;

  if (a == 1024)
    a = 0;

  delay(500);
}
```

## write()

**Description**

Write value to the EEPROM.

**Syntax**

`EEPROM.write(address, value)`

**Parameters**

 - address: the location to write to, starting from 0 (int) to 1023

 - value: the value to write, from 0 to \(2^{32}\) (4 bytes)

**Returns**

None

!!! warning
    The data flash memory has a specified life of write/erase cycles, so you may need to be careful about how often you write to it.

**Example**

``` c++
#include <EEPROM.h>

void setup()
{
  for (int i = 0; i < 255; i++)
    EEPROM.write(i, i);
}

void loop()
{
}
```
## update()

**Description**

Write value to the EEPROM. The value is written only if differs from the one already saved at the same address.

**Syntax**

`EEPROM.update(address, value)`

**Parameters**

 - address: the location to write to, starting from 0 (int) to 1023

 - value: the value to write, from 0 to \(2^{32}\) (4 bytes)

**Returns**

None

!!! warning
    The data flash memory has a specified life of write/erase cycles, using this function instead of [write()](#write) can save cycles if the written data does not change often.

**Example**

``` c++
#include <EEPROM.h>

void setup()
{
  for (int i = 0; i < 255; i++) {
    // this performs as EEPROM.write(i, i)
    EEPROM.update(i, i);
  }
  for (int i = 0; i < 255; i++) {
    // write value "12" to cell 3 only the first time
    // will not write the cell the remaining 254 times
    EEPROM.update(3, 12);
  }
}

void loop()
{
}
```

## get()

**Description**

Read any data type or object from the EEPROM.

**Syntax**

`EEPROM.get(address, data)`

**Parameters**

 - address: the location to read from, starting from 0 (int)

 - data: the data to read, can be a primitive type (eg. float) or a custom struct

**Returns**

A reference to the data passed in

**Example**

``` c++
#include <EEPROM.h>

struct MyObject{
  float field1;
  byte field2;
  char name[10];
};

void setup(){

  float f = 0.00f;   //Variable to store data read from EEPROM.
  int eeAddress = 0; //EEPROM address to start reading from

  Serial.begin( 9600 );
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print( "Read float from EEPROM: " );

  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get( eeAddress, f );
  Serial.println( f, 3 );  //This may print 'ovf, nan' if the data inside the EEPROM is not a valid float.

  // get() can be used with custom structures too. 
  eeAddress = sizeof(float); //Move address to the next byte after float 'f'.
  MyObject customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get( eeAddress, customVar );

  Serial.println( "Read custom object from EEPROM: " );
  Serial.println( customVar.field1 );
  Serial.println( customVar.field2 );
  Serial.println( customVar.name );
}

void loop(){ /* Empty loop */ }
```

## put()

**Description**

Write any data type or object to the EEPROM.

**Syntax**

`EEPROM.put(address, data)`

**Parameters**

 - address: the location to write to, starting from 0 (int)

 - data: the data to write, can be a primitive type (eg. float) or a custom struct

**Returns**

A reference to the data passed in

!!! note
    This function uses EEPROM.update() to perform the write, so does not rewrites the value if it didn't change.

**Example**

``` c++
#include <EEPROM.h>

struct MyObject {
  float field1;
  byte field2;
  char name[10];
};

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  float f = 123.456f;  //Variable to store in EEPROM.
  int eeAddress = 0;   //Location we want the data to be put.


  //One simple call, with the address first and the object second.
  EEPROM.put(eeAddress, f);

  Serial.println("Written float data type!");

  /** Put is designed for use with custom structures also. **/

  //Data to store.
  MyObject customVar = {
    3.14f,
    65,
    "Working!"
  };

  eeAddress += sizeof(float); //Move address to the next byte after float 'f'.

  EEPROM.put(eeAddress, customVar);
  Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
}

void loop() {
  /* Empty loop */
}
```

## eraseAddressBlock()

## fastWrite()

