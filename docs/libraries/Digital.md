!!! note

    The analog input pins can be used as digital pins, referred to as A0, A1, etc.

## pinMode()

**Description**

Configures the specified pin to behave either as an input or an output.


**Syntax**

`pinMode(pin, mode)`


**Parameters**

- pin: the number of the pin whose mode you wish to set

- mode: **INPUT**, **OUTPUT**, or **INPUT_PULLUP**. 


**Returns**

None

**Example**

``` c++
int ledPin = 13;                 // LED connected to digital pin 13

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
}

void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(1000);                  // waits for a second
}
```

## digitalWrite()

**Description**

Write a HIGH or a LOW value to a digital pin.

If the pin has been configured as an OUTPUT with pinMode(), its voltage will be set to the corresponding value: 5V for HIGH, 0V (ground) for LOW.

**Syntax**

`digitalWrite(pin, value)`

**Parameters**

- pin: the pin number

- value: **HIGH** or **LOW**

**Returns**

None

**Example**

``` c++
int ledPin = 13;                 // LED connected to digital pin 13

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
}

void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(1000);                  // waits for a second
}
```
Sets pin 13 to HIGH, makes a one-second-long delay, and sets the pin back to LOW.


## digitalRead()

**Description**

Reads the value from a specified digital pin, either HIGH or LOW.

**Syntax**

`digitalRead(pin)`

**Parameters**

 - pin: the number of the digital pin you want to read (int)

**Returns**

HIGH or LOW

**Example**

Sets pin 13 to the same value as pin 7, declared as an input.

``` c++
int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input
}

void loop()
{
  val = digitalRead(inPin);   // read the input pin
  digitalWrite(ledPin, val);    // sets the LED to the button's value
}
```

!!! note

    If the pin isn't connected to anything, digitalRead() can return either HIGH or LOW (and this can change randomly).

