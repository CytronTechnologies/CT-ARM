## Boards Manager

This is the suggested installation method for end users.

### Prerequisites

- Arduino 1.6.7 and above from [Arduino official website](https://www.arduino.cc/en/Main/Software).
- Internet connection

### Instructions

- Start Arduino IDE and open *Preferences* window.
- Enter `https://cytrontechnologies.github.io/package_cytron_index.json` into **Additional Boards Manager URLs** field. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and find **Cytron ARM Boards by Cytron Technologies Sdn Bhd** platform.
- Select the version you need from a drop-down box.
- Click **install** button.
- After the installation is completed, close the Board Manager window.
- Select your CT-ARM board from Tools > Board menu.

## Using git version

This is the suggested installation method for contributors and library
developers.

### Prerequisites

-  Arduino 1.6.7 (or newer, if you know what you are doing)
-  Python 2.7
-  git
-  terminal, console, or command prompt (depending on you OS)
-  Internet connection

### Instructions

-  Open the console and go to Arduino directory. This can be either your
   **sketchbook** directory (usually `<Documents>/Arduino`), or the
   directory of Arduino application itself, the choice is up to you.
-  Clone this repository into hardware/cytron_arm/ct-arm directory.
   Alternatively, clone it elsewhere and create a symlink, if your OS
   supports them.

```
cd hardware
mkdir -p cytron_arm
cd cytron_arm
git clone https://github.com/CytronTechnologies/CT-ARM.git ct-arm
```

   You should end up with the following directory structure:
```
Arduino
|
--- hardware
   |
   --- cytron_arm
       |
       --- CT-ARM
           |
           --- cores
           --- docs
           --- libraries
           --- tools
           --- package
           --- system
           --- variants
           --- platform.txt
           --- programmers.txt
           --- README.md
           --- boards.txt
           --- LICENSE.md
```

-  Download binary tools

```
cd ct-arm/tools
python get.py
```

-  Restart Arduino
