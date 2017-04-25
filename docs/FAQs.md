!!! note ""
    **Can I use the same program written for Arduino Uno in CT-ARM?**

It depends. at most time it is compatible, unless the program uses libraries which are AVR-specific, then CT-ARM cannot support because it uses ARM architecture.

!!! note ""
    **Can I use the libraries developed for Arduino in CT-ARM program**

Yes, as long as the libraries are not AVR-specific. Mostly basic/common libraries can work in CT-ARM as they should be. Some libraries may be platform-dependent, you can seek for assistance or patches to add CT-ARM platform into library you are using.

!!! note ""
    **What are the advantages of using CT-ARM instead of Arduino Uno?**

CT-ARM provides more RAM and program memory, allowing users to compile larger size of code in their projects. CT-ARM also provides a few more peripherals that Uno doesn't offer, such as more hardware Serials, separate SPI and I2C peripherals from digital I/O pins, etc.

!!! note ""
    **Where can I post the issue if I have problems programming in CT-ARM?**

You can visit our [CT-ARM GitHub page](https://github.com/CytronTechnologies/CT-ARM/issues) to post a new issue. We will get back to you as soon as possible!
