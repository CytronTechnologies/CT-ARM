## analogRead()

**Description**

Reads the value from the specified analog pin. CT-ARM contains 6 channel 12-bit analog to digital converter. However by default, it gives 10 bit resolution, a.k.a it will map input voltages between 0 and 5 volts into integer values between 0 and 1023. This yields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit. To change the resolution, please refer to [analogReadResolution()](#analogreadresolution).

**Syntax**

`analogRead(pin)`

**Parameters**

- pin: the number of the analog input pin to read from 0 to 5


**Returns**

int (0 to 1023)

!!! Note

    If the analog input pin is not connected to anything, the value returned by analogRead() will fluctuate based on a number of factors (e.g. the values of the other analog inputs, how close your hand is to the board, etc.).

**Example**

``` c++
int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
                       // outside leads to ground and +5V
int val = 0;           // variable to store the value read

void setup()
{
  Serial.begin(9600);          //  setup serial
}

void loop()
{
  val = analogRead(analogPin);    // read the input pin
  Serial.println(val);             // debug value
}
```

## analogWrite()
**Description**

Writes an analog value (PWM wave) to a pin. Can be used to light a LED at varying brightnesses or drive a motor at various speeds. After a call to analogWrite(), the pin will generate a steady square wave of the specified duty cycle until the next call to analogWrite() (or a call to digitalRead() or digitalWrite() on the same pin). 

On CT-ARM, this function works on all 16 I/O pins (analog and digital pins).

You do not need to call pinMode() to set the pin as an output before calling analogWrite().

The analogWrite function has nothing to do with the analog pins or the analogRead function.

**Syntax**

`analogWrite(pin, value)`

**Parameters**

 - pin: the pin to write to.

 - value: the duty cycle: between 0 (always off) and 255 (always on).


**Returns**

None

**Example**

``` c++
int ledPin = 13;      // built-in LED
int analogPin = 3;   // potentiometer connected to analog pin 3
int val = 0;         // variable to store the read value

void setup()
{
  pinMode(ledPin, OUTPUT);   // sets the pin as output
}

void loop()
{
  val = analogRead(analogPin);   // read the input pin
  analogWrite(ledPin, val / 4);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}
```

## analogReadResolution()
**Description**

Sets the size (in bits) of the value returned by analogRead(). It defaults to 10 bits (returns values between 0-1023).

CT-ARM has 12-bit ADC capabilities that can be accessed by changing the resolution to 12. This will return values from analogRead() between 0 and 4095.


**Syntax**

`analogReadResolution(bits)`


**Parameters**

- bits: determines the resolution (in bits) of the value returned by analogRead() function. You can set this 1 and 12.

**Returns**

None

!!! Note

    If you set the analogReadResolution() value to a value lower than your board's capabilities, the extra least significant bits read from the ADC will be discarded.

**Example**

``` c++
void setup() {
  // open a serial connection
  Serial.begin(9600); 
}

void loop() {
  // read the input on A0 at default resolution (10 bits)
  // and send it out the serial connection 
  analogReadResolution(10);
  Serial.print("ADC 10-bit (default) : ");
  Serial.print(analogRead(A0));

  // change the resolution to 12 bits and read A0
  analogReadResolution(12);
  Serial.print(", 12-bit : ");
  Serial.print(analogRead(A0));

  // change the resolution to 8 bits and read A0
  analogReadResolution(8);
  Serial.print(", 8-bit : ");
  Serial.println(analogRead(A0));

  // a little delay to not hog serial monitor
  delay(100);
}
```

## analogWriteResolution()
**Description**

Sets the resolution of the analogWrite() function. It defaults to 8 bits (values between 0-255) 


**Syntax**

`analogWriteResolution(bits)`


**Parameters**

- bits: determines the resolution (in bits) of the values used in the analogWrite() function.


**Returns**

None

**Example**

``` c++
void setup(){
  // open a serial connection
  Serial.begin(9600); 
  // make our digital pin an output
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop(){
  // read the input on A0 and map it to a PWM pin
  // with an attached LED
  int sensorVal = analogRead(A0);
  Serial.print("Analog Read) : ");
  Serial.print(sensorVal);

  // the default PWM resolution
  analogWriteResolution(8);
  analogWrite(11, map(sensorVal, 0, 1023, 0 ,255));
  Serial.print(" , 8-bit PWM value : ");
  Serial.print(map(sensorVal, 0, 1023, 0 ,255));

  // change the PWM resolution to 12 bits
  // the full 12 bit resolution is only supported
  // on the Due
  analogWriteResolution(12);
  analogWrite(12, map(sensorVal, 0, 1023, 0, 4095));
  Serial.print(" , 12-bit PWM value : ");
  Serial.print(map(sensorVal, 0, 1023, 0, 4095));

  // change the PWM resolution to 4 bits
  analogWriteResolution(4);
  analogWrite(13, map(sensorVal, 0, 1023, 0, 15));
  Serial.print(", 4-bit PWM value : ");
  Serial.println(map(sensorVal, 0, 1023, 0, 15));

  delay(5);

```
