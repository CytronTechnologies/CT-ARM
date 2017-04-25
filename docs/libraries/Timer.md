CT-ARM has 4 built-in hardware timers: **Timer1**, **Timer2**, **Timer3** and **Timer4**. These timers do not interfere with functions like [delay()](), [delayMicroseconds()](), [millis()]() and [micros()](). This Timer library allows users to use timer interrupts for program routines.

!!! note
    Tone library uses Timer2, so make sure that you do not use Timer2 interrupts with function [Tone()]()

## initialize()

**Description**

You must call this method first to use any of the other methods. You can optionally specify the timer's period here (in microseconds), by default it is set at 1 second.

**Syntax**

`Timer1.initialize(period)`

Also applies to **Timer2**, **Timer3** and **Timer4**.

**Parameters**

 - period: timer's period in microseconds, ranges from 2 to 16,777,216

**Return**

None

## attachInterrupt()

**Description**

Calls a function at the specified time interval in microseconds. The time interval is set from [initialize()](#initialize) function. Be careful about trying to execute too complicated of an interrupt at too high of a frequency, or the CPU may never enter the main loop and your program will 'lock up'. 

**Syntax**

`Timer1.attachInterrupt(handler)`

Also applies to **Timer2**, **Timer3** and **Timer4**.

**Parameters**

 - handler: the ISR to call when the interrupt occurs; this function must take no parameters and return nothing. This function is sometimes referred to as an interrupt service routine.

**Return**

None

**Example**
```c++
void setup()
{
  pinMode(10, OUTPUT);
  Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
 
void callback()
{
  digitalWrite(10, digitalRead(10) ^ 1);
}
 
void loop()
{
  // your program here...
}
```

## detachInterrupt()

**Description**

Disables the attached interrupt.

**Syntax**

`Timer1.detachInterrupt()`

Also applies to **Timer2**, **Timer3** and **Timer4**.

**Parameters**

None

**Return**

None
