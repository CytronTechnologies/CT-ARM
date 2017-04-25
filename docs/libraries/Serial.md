Serial is used for communication between the board and a computer or other devices. CT-ARM has one main serial port (also known as a UART or USART): **Serial**. It communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB. Thus, if you use these functions, you cannot also use pins 0 and 1 for digital input or output.

You can use the Arduino environment's built-in serial monitor to communicate with an Arduino board. Click the serial monitor button in the toolbar and select the same baud rate used in the call to begin().

CT-ARM has 4 additional serial ports: **Serial2** on pins 8 (RX) and 9 (TX), **Serial3** on pins A4 (RX) and A5 (TX), **Serial4** on pins SCL (RX) and SDA (TX) , and **Serial5** on pins 10 (RX) and 11 (TX). To use these pins to communicate with your personal computer, you will need an additional USB-to-serial adaptor, as they are not connected to the CT-ARM's USB-to-serial adaptor. To use them to communicate with an external TTL serial device, connect the TX pin to your device's RX pin, the RX to your device's TX pin, and the ground of your CT-ARM to your device's ground.

CT-ARM also has 1 special port: **Serial1**, which shares same RX and TX pins as Serial. To use Serial1, just simply cut the track between pad UART0 and center pad at the back of CT-ARM board, then solder pad UART1 and center pad together. The advantage is that pin 0 and 1 are no longer interferring with Serial debugging, and the users can use these 2 pins for another extra TTL serial device.

The UART pinouts are listed as below:

|UART|RX|TX|
|---|---|---|
|Serial|0 (default)|1 (default)|
|Serial1|0|1|
|Serial2|8|9|
|Serial3|A4|A5|
|Serial4|SCL|SDA|
|Serial5|10|11|

!!! warning
    Serial communication on pins TX/RX on CT-ARM uses TTL logic levels 5V. Don't connect these pins directly to an RS232 serial port; they operate at +/- 12V and can damage your CT-ARM board. 

## If (Serial)

**Description**

Indicates if the specified Serial port is ready.

**Syntax**

`If(Serial)`

`If(Serial1)`

`If(Serial2)`

`If(Serial3)`

`If(Serial4)`

`If(Serial5)`

**Parameters**

None

**Return**

boolean : returns true if the specified serial port is available.

**Example**

```c++
void setup() { 
 //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
} 

void loop() { 
 //proceed normally
} 
```

## available()

**Description**

Get the number of bytes (characters) available for reading from the serial port. This is data that's already arrived and stored in the serial receive buffer (which holds 64 bytes). available() inherits from the Stream utility class.

**Syntax**

`Serial.available()`

`Serial1.available()`

`Serial2.available()`

`Serial3.available()`

`Serial4.available()`

`Serial5.available()`

**Parameters**

None

**Return**

the number of bytes available to read

**Example**

Serial Example:

```c++
int incomingByte = 0;   // for incoming serial data

void setup() {
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
}

void loop() {

  // send data only when you receive data:
  if (Serial.available() > 0) {
     // read the incoming byte:
     incomingByte = Serial.read();

     // say what you got:
     Serial.print("I received: ");
     Serial.println(incomingByte, DEC);
  }
}
```

Serial2 Example:

```c++
void setup() {
  // opens port 0 and port 2, both sets data rate to 9600 bps
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  // read from port 0, send to port 2:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial2.print(inByte, BYTE); 

  }
  // read from port 2, send to port 0:
  if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.print(inByte, BYTE); 
  }
}
```

## begin()

**Description**

Sets the data rate in bits per second (baud) for serial data transmission. For communicating with the computer, use one of these rates: 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200. You can, however, specify other rates - for example, to communicate over pins 0 and 1 with a component that requires a particular baud rate.

**Syntax**

`Serial.begin()`

`Serial1.begin()`

`Serial2.begin()`

`Serial3.begin()`

`Serial4.begin()`

`Serial5.begin()`

**Parameters**

 - speed: in bits per second (baud) - long

**Return**

None

**Example**

```c++
void setup() {
    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
    Serial2.begin(19200); // opens serial port 2 , sets data rate to 19200 bps
    Serial5.begin(4800); // opens serial port 5, sets data rate to 4800 bps
}

void loop() {}
```

## end()

**Description**

Disables serial communication, allowing the RX and TX pins to be used for general input and output. To re-enable serial communication, call Serial.begin().

**Syntax**

`Serial.end`

`Serial1.end`

`Serial2.end`

`Serial3.end`

`Serial4.end`

`Serial5.end`

**Parameters**

None

**Return**

None

## find()

**Description**

Serial.find() reads data from the serial buffer until the target string of given length is found. The function returns true if target string is found, false if it times out.

Serial.find() inherits from the Stream utility class.

**Syntax**

`Serial.find(target)`

**Parameters**

 - target : the string to search for (char)

**Return**

boolean

## findUntil()

**Description**

Serial.findUntil() reads data from the serial buffer until a target string of given length or terminator string is found.

The function returns true if the target string is found, false if it times out.

Serial.findUntil() inherits from the Stream utility class.

**Syntax**

`Serial.findUntil(target, terminal)`

**Parameters**

 - target : the string to search for (char)
 - terminal : the terminal string in the search (char)

**Return**

boolean

## flush()

**Description**

Waits for the transmission of outgoing serial data to complete. (Prior to Arduino 1.0, this instead removed any buffered incoming serial data.)

flush() inherits from the Stream utility class.

**Syntax**

`Serial.flush()`

**Parameters**

None

**Return**

None

## peek()

**Description**

Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer. That is, successive calls to peek() will return the same character, as will the next call to read(). peek() inherits from the Stream utility class.

**Syntax**

`Serial.peek()`

**Parameters**

None

**Return**

the first byte of incoming serial data available (or -1 if no data is available) - int

## print()

**Description**

Prints data to the serial port as human-readable ASCII text. This command can take many forms. Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, defaulting to two decimal places. Bytes are sent as a single character. Characters and strings are sent as is. For example:

 * Serial.print(78) gives "78"
 * Serial.print(1.23456) gives "1.23"
 * Serial.print('N') gives "N"
 * Serial.print("Hello world.") gives "Hello world."

An optional second parameter specifies the base (format) to use; permitted values are BIN (binary, or base 2), OCT (octal, or base 8), DEC (decimal, or base 10), HEX (hexadecimal, or base 16). For floating point numbers, this parameter specifies the number of decimal places to use. For example:

 * Serial.print(78, BIN) gives "1001110"
 * Serial.print(78, OCT) gives "116"
 * Serial.print(78, DEC) gives "78"
 * Serial.print(78, HEX) gives "4E"
 * Serial.println(1.23456, 0) gives "1"
 * Serial.println(1.23456, 2) gives "1.23"
 * Serial.println(1.23456, 4) gives "1.2346"

You can pass flash-memory based strings to Serial.print() by wrapping them with F(). For example :

 * Serial.print(F(“Hello World”))

To send a single byte, use [Serial.write()](#write).

**Syntax**

`Serial.print(val)`

`Serial.print(val, format)`

**Parameters**

 - val: the value to print - any data type

 - format: specifies the number base (for integral data types) or number of decimal places (for floating point types)

**Return**

size_t (long): print() returns the number of bytes written, though reading that number is optional

**Example**

```c++
/*
Uses a FOR loop for data and prints a number in various formats.
*/
int x = 0;    // variable

void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps:    
}

void loop() {  
  // print labels 
  Serial.print("NO FORMAT");       // prints a label
  Serial.print("\t");              // prints a tab

  Serial.print("DEC");  
  Serial.print("\t");      

  Serial.print("HEX"); 
  Serial.print("\t");   

  Serial.print("OCT");
  Serial.print("\t");

  Serial.print("BIN");
  Serial.print("\t"); 

  for(x=0; x< 64; x++){    // only part of the ASCII chart, change to suit

    // print it out in many formats:
    Serial.print(x);       // print as an ASCII-encoded decimal - same as "DEC"
    Serial.print("\t");    // prints a tab

    Serial.print(x, DEC);  // print as an ASCII-encoded decimal
    Serial.print("\t");    // prints a tab

    Serial.print(x, HEX);  // print as an ASCII-encoded hexadecimal
    Serial.print("\t");    // prints a tab

    Serial.print(x, OCT);  // print as an ASCII-encoded octal
    Serial.print("\t");    // prints a tab

    Serial.println(x, BIN);  // print as an ASCII-encoded binary
    //                             then adds the carriage return with "println"
    delay(200);            // delay 200 milliseconds
  }
  Serial.println("");      // prints another carriage return
}
```

## println()

**Description**

Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n'). This command takes the same forms as [Serial.print()](#print).

**Syntax**

`Serial.println(val)`

`Serial.println(val, format)`

**Parameters**

 - val: the value to print - any data type

 - format: specifies the number base (for integral data types) or number of decimal places (for floating point types)

**Return**

size_t (long): println() returns the number of bytes written, though reading that number is optional

**Example**

```c++
/*
  Analog input

 reads an analog input on analog in 0, prints the value out.

 created 24 March 2006
 by Tom Igoe
 */

int analogValue = 0;    // variable to hold the analog value

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog input on pin 0:
  analogValue = analogRead(0);

  // print it out in many formats:
  Serial.println(analogValue);       // print as an ASCII-encoded decimal
  Serial.println(analogValue, DEC);  // print as an ASCII-encoded decimal
  Serial.println(analogValue, HEX);  // print as an ASCII-encoded hexadecimal
  Serial.println(analogValue, OCT);  // print as an ASCII-encoded octal
  Serial.println(analogValue, BIN);  // print as an ASCII-encoded binary

  // delay 10 milliseconds before the next reading:
  delay(10);
}
```

## read()

**Description**

Reads incoming serial data. [read()](#read) inherits from the Stream utility class.

**Syntax**

`Serial.read()`

`Serial1.read()`

`Serial2.read()`
 
`Serial3.read()`

`Serial4.read()`

`Serial5.read()`

**Parameters**

None

**Return**

the first byte of incoming serial data available (or -1 if no data is available) - int

**Example**

```c++
int incomingByte = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}
```

## readBytes()

**Description**

Serial.readBytes() reads characters from the serial port into a buffer. The function terminates if the determined length has been read, or it times out (see [Serial.setTimeout()](#settimeout)).

Serial.readBytes() returns the number of characters placed in the buffer. A 0 means no valid data was found.

Serial.readBytes() inherits from the Stream utility class.

**Syntax**

`Serial.readBytes(buffer, length)`

**Parameters**

 - buffer: the buffer to store the bytes in (char[] or byte[])
 - length : the number of bytes to read (int)

**Return**

byte

## readBytesUntil()

**Description**

Serial.readBytesUntil() reads characters from the serial buffer into an array. The function terminates if the terminator character is detected, the determined length has been read, or it times out (see [Serial.setTimeout()](#settimeout)).

Serial.readBytesUntil() returns the number of characters read into the buffer. A 0 means no valid data was found.

Serial.readBytesUntil() inherits from the Stream utility class.

**Syntax**

`Serial.readBytesUntil(character, buffer, length)`

**Parameters**

 - character : the character to search for (char)
 - buffer: the buffer to store the bytes in (char[] or byte[])
 - length : the number of bytes to read (int)

**Return**

byte

## readString()

**Description**

Reads characters from the serial buffer into a string. The function terminates if it times out (see [setTimeout()](#settimeout)).

**Syntax**

`Serial.readString()`

**Parameters**

None

**Return**

A string read from the serial buffer

## readStringUntil()

**Description**

Reads characters from the serial buffer into a string. The function terminates if the terminator character is detected or it times out (see [setTimeout()](#settimeout)).

**Syntax**

Serial.readStringUntil(terminator)

**Parameters**

 - terminator : the character to search for (char)

**Return**

The entire string read from the serial buffer, until the terminator character is detected

## setTimeout()

**Description**

Sets the maximum milliseconds to wait for serial data when using [Serial.readBytesUntil()](#readbytesuntil) and  [Serial.readBytes()](#readbytes). It defaults to 1000 milliseconds.

**Syntax**

`Serial.setTimeout(time)`

**Parameters**

 - time : timeout duration in milliseconds (long).

**Return**

None

## write()

**Description**

Writes binary data to the serial port. This data is sent as a byte or series of bytes; to send the characters representing the digits of a number use the [print()](#print) function instead.

**Syntax**

`Serial.write(val)`

`Serial.write(str)` 

`Serial.write(buf, len)`

Also supports: **Serial1**, **Serial2**, **Serial3**, **Serial4** and **Serial5** (in place of Serial)

**Parameters**

 - val: a value to send as a single byte

 - str: a string to send as a series of bytes

 - buf: an array to send as a series of bytes

 - len: the length of the buffer

**Return**

byte

> [write()](#write) will return the number of bytes written, though reading that number is optional

**Example**

```c++
void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.write(45); // send a byte with the value 45

   int bytesSent = Serial.write(“hello”); //send the string “hello” and return the length of the string.
}
```

## serialEvent()

**Description**

Called when data is available. Use [Serial.read()](#read) to capture this data.

**Syntax**

```c++
void serialEvent(){
//statements
}

void serialEvent1(){
//statements
}

void serialEvent2(){
//statements
}

void serialEvent3(){
//statements
}

void serialEvent4(){
//statements
}

void serialEvent5(){
//statements
}
```

**Parameters**

statements: any valid statements

