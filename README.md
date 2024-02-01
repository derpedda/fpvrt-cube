# FPVRT-Cube
This project contains the PlatformIO-Code for the ESP32-based companion used in the [FPVRT-Project](https://github.com/derpedda/fpvrt).

## Project
This project was first developed as part of the Quadcopter Seminar of [GippLab](https://gipplab.org/teaching/fpv-quadcopter/) at the University of Göttingen in the winter term of 2023/24 by DerPedda.

## Goal
Develop a cheap, lightweight and opensourced hardware-companion that can be strapped on an FPV-Quadcopter to assist in measuring race laps.

## Hardware 
For the hardware part, M5Stack Stamps are used as a small and lightweight formfactor for the ESP32-S3 chip.
With the ESP32-S3, it's Bluetooth-capabilities get used to provide a BLE-Beacon using the Eddystone specification that is regognizable by the FPVRT-APP.
Additionally, with the help of 2 WS2812B LEDs, lighting a dome made of half a tabletennis ball, an optical identifier is created.

## Acknowledgements
Using [PlatformIO](https://github.com/platformio) and the [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
