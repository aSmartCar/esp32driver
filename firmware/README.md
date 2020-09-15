# ESP32-Driver Firmware

## Requierd software

- VSCode
    - PlatformIO
- yarn
    - nodejs
- (optional) git

## Installation

Clone or download the repositories

[Firmware](https://nextlab-i.ostfalia.de/bachelorarbeit-koehler/esp-firmware/firmware)

[Configuration Website Embedded](https://nextlab-i.ostfalia.de/bachelorarbeit-koehler/esp-firmware/configwebsite)

### Firmware Configuration

1. Open the firmware folder with VSCode.
2. Open the config.json file located in the data folder.
3. Set your preferd configuration.

### Embedded-Website preparation

1. Open the folder with VSCode or with a terminal
2. Install the requierd nodepackages:
    ```yarn install```
3. Build the projekt:
    ```yarn run build```
4. Copy the foldersturcture from the dist folder to the data folder inside the firmware projekt.

### Flash all files

1. Open an platformio terminal inside the vscode window from the firmware folder.
2. Run the command ```pio run --target uploadfs```
3. Run the command ```pio run --target upload```

## Sensors
The akku voltage measurement uses resistors to measure the akku voltage. The default values are 12 volts for akku0 and 20 volts for akku1. Do not use a higher voltage to prevent damage on the ESP32.


