## attachInterrupt()

**Description**

> Digital Pins With Interrupts

All I/O pins on CT-ARM can be used as external interrupts. You can place pin number as first parameter of attachInterrupt. For example, if you connect to pin 3, use 3 as first parameter to attachInterrupt.

> Using Interrupts

Interrupts are useful for making things happen automatically in microcontroller programs, and can help solve timing problems. Good tasks for using an interrupt may include reading a rotary encoder, or monitoring user input.

If you wanted to insure that a program always caught the pulses from a rotary encoder, so that it never misses a pulse, it would make it very tricky to write a program to do anything else, because the program would need to constantly poll the sensor lines for the encoder, in order to catch pulses when they occurred. Other sensors have a similar interface dynamic too, such as trying to read a sound sensor that is trying to catch a click, or an infrared slot sensor (photo-interrupter) trying to catch a coin drop. In all of these situations, using an interrupt can free the microcontroller to get some other work done while not missing the input.

> Interrupt Service Routines

ISRs are special kinds of functions that have some unique limitations most other functions do not have. An ISR cannot have any parameters, and they shouldn't return anything.

Generally, an ISR should be as short and fast as possible. If your sketch uses multiple ISRs, only one can run at a time, other interrupts will be executed after the current one finishes in an order that depends on the priority they have. millis() relies on interrupts to count, so it will never increment inside an ISR. Since delay() requires interrupts to work, it will not work if called inside an ISR. micros() works initially, but will start behaving erratically after 1-2 ms. delayMicroseconds() does not use any counter, so it will work as normal.

Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

For more information on interrupts, see [Nick Gammon's notes](http://gammon.com.au/interrupts).

!!! warning
    
    Inside the attached function, delay() won't work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

**Syntax**

`attachInterrupt(pin, ISR, mode)`

**Parameters**

 - pin: the pin number
 - ISR: the ISR to call when the interrupt occurs; this function must take no parameters and return nothing. This function is sometimes referred to as an interrupt service routine.
 - mode: defines when the interrupt should be triggered. 3 constants are predefined as valid values:
	* **CHANGE** to trigger the interrupt whenever the pin changes value
	* **RISING** to trigger when the pin goes from low to high,
	* **FALLING** for when the pin goes from high to low.

**Returns**

None

**Example**

``` c++
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin, blink, CHANGE);
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  state = !state;
}
```

## detachInterrupt()

**Description**

Turns off the given interrupt.

**Syntax**

`detachInterrupt(pin)`

**Parameters**

 - pin: the pin number of the interrupt to disable 

**Returns**

None

**Example**

``` c++
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
int count = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin, blink, CHANGE);
}

void loop() {
  digitalWrite(ledPin, state);
  if(count > 10){ // stop the blinking after 10 times
    detachInterrupt(interruptPin);
  }
}

void blink() {
  state = !state;
  if(state == HIGH){
   count++;
  }
}
```
