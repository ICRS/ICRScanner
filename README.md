# 💳 ICRS Card Scanner

## [💻 MAIN CODEBASE: https://github.com/ICRS/icrs_lab](https://github.com/ICRS/icrs_lab)

## 👋 Introduction

This is used to verify the identity of students who are using the 3D printer in the ICRS lab. By displaying information about them based on their ID card.

## 📦 Features

- Student verification
- 3D printing Authentication
- Adding new card to the system (not implemented yet)

[//]: <> ( TODO: add images )

## 🔨 Build and Deployment Instructions

See [https://github.com/ICRS/icrs_lab](https://github.com/ICRS/icrs_lab)

copy the contents of `src/TFT_config.h` into `.pio/libdeps/lilygo-t-display/TFT_eSPI/User_Setup.h`

This will let us use the settings for teh TFT display with the library

Then use [Platform IO](https://platformio.org/) to build and upload the code.

### Hardware

- [ESP32 lilygo-t-display](https://lilygo.cc/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board)
- [PN532 NFC module](https://www.nxp.com/products/rfid-nfc/nfc-hf/nfc-readers/nfc-integrated-solution:PN5321A3HN)

## 📝 Paper

https://www.researchgate.net/publication/385302645_Automated_Student_3D_Printing_Verification_Process
