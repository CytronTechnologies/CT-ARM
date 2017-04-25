## SPISettings

**Description**

The SPISettings object is used to configure the SPI port for your SPI device. All 3 parameters are combined to a single SPISettings object, which is given to SPI.beginTransaction().

**Syntax**

`SPISettings mySetting(speedMaximum, dataOrder, dataMode)`

`SPI.beginTransaction(mySetting)`

**Parameters**

 - speedMaximum: The maximum speed of communication. For a SPI chip rated up to 20 MHz, use 20000000.

 - dataOrder: MSBFIRST or LSBFIRST

 - dataMode : SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3

**Return**

None

## begin()

**Description**

Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.

**Syntax**

`SPI.begin()`

**Parameters**

None

**Return**

None

**Example**

## end()

**Description**

Disables the SPI bus (leaving pin modes unchanged).

**Syntax**

`SPI.end()`

**Parameters**

None

**Return**

None

## beginTransaction()

**Description**

Initializes the SPI bus using the defined [SPISettings](#spisettings).

**Syntax**

`SPI.beginTransaction(mySettings);`

**Parameters**

 - mySettings: the chosen settings according to SPISettings.

**Return**

None

## endTransaction()

**Description**

Stop using the SPI bus. Normally this is called after de-asserting the chip select, to allow other libraries to use the SPI bus.

**Syntax**

`SPI.endTransaction()`

**Parameters**

None

**Return**

None

## setBitOrder()

**Description**

Sets the order of the bits shifted out of and into the SPI bus, either LSBFIRST (least-significant bit first) or MSBFIRST (most-significant bit first).

**Syntax**

`SPI.setBitOrder(order)`

**Parameters**

 - order: either LSBFIRST or MSBFIRST

**Return**

None

## setClockDivider()

**Description**

Sets the SPI clock divider relative to the system clock. The dividers available are 2, 4, 8, 16, 32, 64 or 128. The default setting is SPI_CLOCK_DIV4, which sets the SPI clock to one-quarter the frequency of the Arduino system clock (4 Mhz for the boards at 16 MHz).

**Syntax**

`SPI.setClockDivider(divider)`

**Parameters**

 * divider:
	* SPI_CLOCK_DIV2 (8MHz)
	* SPI_CLOCK_DIV4 (4MHz, default)
	* SPI_CLOCK_DIV8 (2MHz)
	* SPI_CLOCK_DIV16 (1MHz)
	* SPI_CLOCK_DIV32 (500kHz) 
	* SPI_CLOCK_DIV64 (250kHz)
	* SPI_CLOCK_DIV128 (125kHz)

**Return**

None

## setDataMode()

**Description**

Sets the SPI data mode: that is, clock polarity and phase. See the [Wikipedia article on SPI](http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus) for details.

**Syntax**

`SPI.setDataMode(mode)`

**Parameters**

 * mode:	
 	* SPI_MODE0
 	* SPI_MODE1
 	* SPI_MODE2
 	* SPI_MODE3

**Return**

None

## transfer()

**Description**

SPI transfer is based on a simultaneous send and receive: the received data is returned in receivedVal (or receivedVal16). In case of buffer transfers the received data is stored in the buffer in-place (the old data is replaced with the data received).

**Syntax**

`receivedVal = SPI.transfer(val)`

`SPI.transfer(buffer, size)`

**Parameters**

 - val: the byte to send out over the bus

 - buffer: the array of data to be transferred

**Return**

the received data
