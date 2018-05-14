## 1.1.0 (16/4/2018)

### **Bug Fixes**
- Fixed program memory sudden rise issue during program compilation
- Retidy Servo library

### **Changes**
- Change pin mapping of analog, I2C and SPI, but still backward compatible

### **New**
- Added Firmata compatibility

## 1.0.1 (18/4/2017)

### **Bug Fixes**

 - External interrupts now can be applied on all pins
 - SoftwareSerial now can be used on all pins, tested works stable at 9600 baud or lower
 - Fixed the analogReadResolution issue, by default now gives 10 bit resolution

### **New**

 - Added ShiftIn and ShiftOut
