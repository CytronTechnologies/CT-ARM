The SoftwareSerial library has been developed to allow serial communication on other digital pins of CT-ARM, using software to replicate the functionality of native hardware Serial (hence the name "SoftwareSerial"). In CT-ARM, all I/O pins can be used for SoftwareSerial.

However, there is a few limitations:

 - If using multiple software serial ports, only one can receive data at a time.
 - Higher the baud rate, the less stable the data reception on RX pins. 9600 baud or below is the most stable baud rate tested for data transmission. For higher baud rate needed in applications, [Serial](/libraries/Serial) would be recommended.

## SoftwareSerial()

**Description**

SoftwareSerial is used to create an instance of a SoftwareSerial object, whose name you need to provide as in the example below. The inverse_logic argument is optional and defaults to false. See below for more details about what it does. Multiple SoftwareSerial objects may be created, however only one can be active at a given moment.

You need to call [begin()](#begin) to enable communication.

**Syntax**

`SoftwareSerial mySerial(rxPin, txPin)`

**Parameters**

 - rxPin: the pin on which to receive serial data

 - txPin: the pin on which to transmit serial data

 - inverse_logic: is used to invert the sense of incoming bits (the default is normal logic). If set, SoftwareSerial treats a LOW (0 volts on the pin, normally) on the Rx pin as a 1-bit (the idle state) and a HIGH (5 volts on the pin, normally) as a 0-bit. It also affects the way that it writes to the Tx pin. Default value is false.

**Example**

```c++
#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;

// set up a new serial object
SoftwareSerial mySerial (rxPin, txPin);
```

## available()

**Description**

Get the number of bytes (characters) available for reading from a software serial port. This is data that's already arrived and stored in the serial receive buffer.

**Syntax**

`mySerial.available()`

**Parameters**

None

**Return**

the number of bytes available to read

**Example**
```c++
// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

// set up a new serial port
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available()>0){
    mySerial.read();
  }
}
```

## begin()

**Description**

Sets the speed (baud rate) for the serial communication. Supported baud rates are 1200, 2400, 4800, 9600, 19200, 38400, 57600, and 115200.

**Syntax**

`mySerial.begin(speed)`

**Parameters**

 - speed: the baud rate (long)

**Return**

None

**Example**
```c++
// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

// set up a new serial port
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() {
  // ...
}
```

## isListening()

**Description**

Tests to see if requested software serial port is actively listening.

**Syntax**

`mySerial.isListening()`

**Parameters**

None

**Return**

boolean

**Example**
```c++
#include <SoftwareSerial.h>

// software serial : TX = digital pin 10, RX = digital pin 11
SoftwareSerial portOne(10,11);

void setup()
{
  // Start the hardware serial port
  Serial.begin(9600);

  // Start software serial port
  portOne.begin(9600);
}

void loop()
{
  if (portOne.isListening()) {
   Serial.println("Port One is listening!"); 
}
```

## overflow()

**Description**

Tests to see if a software serial buffer overflow has occurred. Calling this function clears the overflow flag, meaning that subsequent calls will return false unless another byte of data has been received and discarded in the meantime.

The software serial buffer can hold 64 bytes.

**Syntax**

`mySerial.overflow()`

**Parameters**

None

**Return**

boolean

**Example**
```c++
#include <SoftwareSerial.h>

// software serial : TX = digital pin 10, RX = digital pin 11
SoftwareSerial portOne(10,11);

void setup()
{
  // Start the hardware serial port
  Serial.begin(9600);

  // Start software serial port
  portOne.begin(9600);
}

void loop()
{
  if (portOne.overflow()) {
   Serial.println("SoftwareSerial overflow!"); 
}
```

## peek()

**Description**

Return a character that was received on the RX pin of the software serial port. Unlike read(), however, subsequent calls to this function will return the same character.

Note that only one SoftwareSerial instance can receive incoming data at a time (select which one with the [listen()](#listen) function).

**Syntax**

`mySerial.peek()`

**Parameters**

None

**Return**

the character read, or -1 if none is available

**Example**
```c++
SoftwareSerial mySerial(10,11);

void setup()
{
  mySerial.begin(9600);
}

void loop()
{
  char c = mySerial.peek();
}
```

## read()

**Description**

Return a character that was received on the RX pin of the software serial port. Note that only one SoftwareSerial instance can receive incoming data at a time (select which one with the [listen()](#listen) function).

**Syntax**

`mySerial.read()`

**Parameters**

None

**Return**

the character read, or -1 if none is available

**Example**
```c++
SoftwareSerial mySerial(10,11);

void setup()
{
  mySerial.begin(9600);
}

void loop()
{
  char c = mySerial.read();
}
```

## print()

**Description**

Prints data to the transmit pin of the software serial port. Works the same as the [Serial.print()](/libraries/Serial/#print) function.

**Syntax**

`mySerial.print()`

**Parameters**

vary, see [Serial.print()](/libraries/Serial/#print) for details

**Return**

byte

> [print()](#print) will return the number of bytes written, though reading that number is optional

**Example**
```c++
SoftwareSerial serial(10,11);
int analogValue;

void setup()
{
  serial.begin(9600);
}

void loop()
{
  // read the analog input on pin 0:
  analogValue = analogRead(A0);

  // print it out in many formats:
  serial.print(analogValue);         // print as an ASCII-encoded decimal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, DEC);    // print as an ASCII-encoded decimal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, HEX);    // print as an ASCII-encoded hexadecimal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, OCT);    // print as an ASCII-encoded octal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, BIN);    // print as an ASCII-encoded binary
  serial.print("\t");                // print a tab character
  serial.print(analogValue/4, BYTE); // print as a raw byte value (divide the
                                     // value by 4 because analogRead() returns numbers
                                     // from 0 to 1023, but a byte can only hold values
                                     // up to 255)
  serial.print("\t");                // print a tab character    
  serial.println();                  // print a linefeed character

  // delay 10 milliseconds before the next reading:
  delay(10);
}
```

## println()

**Description**

Prints data to the transmit pin of the software serial port, followed by a carriage return and line feed. Works the same as the [Serial.println()](/libraries/Serial/#println) function.

**Syntax**

`mySerial.println()`

**Parameters**

vary, see [Serial.println()](/libraries/Serial/#println) for details

**Return**

byte

> [println()](#println) will return the number of bytes written, though reading that number is optional

**Example**
```c++
SoftwareSerial serial(10,11);
int analogValue;

void setup()
{
  serial.begin(9600);
}

void loop()
{
  // read the analog input on pin 0:
  analogValue = analogRead(A0);

  // print it out in many formats:
  serial.print(analogValue);         // print as an ASCII-encoded decimal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, DEC);    // print as an ASCII-encoded decimal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, HEX);    // print as an ASCII-encoded hexadecimal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, OCT);    // print as an ASCII-encoded octal
  serial.print("\t");                // print a tab character
  serial.print(analogValue, BIN);    // print as an ASCII-encoded binary
  serial.print("\t");                // print a tab character
  serial.print(analogValue/4, BYTE); // print as a raw byte value (divide the
                                     // value by 4 because analogRead() returns numbers
                                     // from 0 to 1023, but a byte can only hold values
                                     // up to 255)
  serial.print("\t");                // print a tab character    
  serial.println();                  // print a linefeed character

  // delay 10 milliseconds before the next reading:
  delay(10);
}
```

## listen()

**Description**

Enables the selected software serial port to listen. Only one software serial port can listen at a time; data that arrives for other ports will be discarded. Any data already received is discarded during the call to listen() (unless the given instance is already listening).

**Syntax**

`mySerial.listen()`

**Parameters**

`mySerial:the name of the instance to listen`

**Return**

None

**Example**

```c++
#include <SoftwareSerial.h>

// software serial : TX = digital pin 10, RX = digital pin 11
SoftwareSerial portOne(10, 11);

// software serial : TX = digital pin 8, RX = digital pin 9
SoftwareSerial portTwo(8, 9);

void setup()
{
  // Start the hardware serial port
  Serial.begin(9600);

  // Start both software serial ports
  portOne.begin(9600);
  portTwo.begin(9600);

}

void loop()
{
  portOne.listen();

  if (portOne.isListening()) {
   Serial.println("Port One is listening!"); 
}else{
   Serial.println("Port One is not listening!"); 
}

  if (portTwo.isListening()) {
   Serial.println("Port Two is listening!"); 
}else{
   Serial.println("Port Two is not listening!"); 
}

}
```

## write()

**Description**

Prints data to the transmit pin of the software serial port as raw bytes. Works the same as the [Serial.write()](/libraries/Serial/#write) function.

**Syntax**

`mySerial.write(data)`

**Parameters**

 - data: refer to [Serial.write()](/libraries/Serial/#write) for details

**Return**

byte

> [write()](#write) will return the number of bytes written, though reading that number is optional

**Example**
```c++
SoftwareSerial mySerial(10, 11);

void setup()
{
  mySerial.begin(9600);
}

void loop()
{
  // send a byte with the value 45
  mySerial.write(45);

  //send the string “hello” and return the length of the string.
  int bytesSent = mySerial.write(“hello”);
}
```
