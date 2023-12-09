# ECE 47700 Team 10 - Sign2Sound
Sign2Sound is a semi-portable ASL translator, which includes the device itself and a color-coded right-handed glove. As a predecessor to a more advanced device, Sign2Sound only recognizes the 24 stationary letters of the ASL alphabet for a right-handed individual. First, the device streams feedback from a camera to a screen, allowing the user to frame their gloved hand properly. The glove is used to standardize differences in the users’ hand shape, size, and color and make sign identification easier. Once the user is ready, they can press a button to trigger image capture. The image is processed on a RaspberryPi that houses a team-designed computer vision algorithm, associating signs with their English alphabet translation. Then, the translation is sent to our microcontroller via UART. With the identified letter data, the microcontroller manages two forms of output, audio and visual. For the audio output, the microcontroller transmits the identified letter’s audio file to a DAC via I2C. Subsequently, the converted analog signal is output from the DAC to a speaker using a team-designed amplifier circuit. As for the visual output, the microcontroller transmits the identified letter to an OLED LCD display via SPI. The device will be powered by a wall wart transformer, supplying 5V directly to the RaspberryPi. Power will be fed from the RaspberryPi to a LDO to reduce the voltage to 3.3V for the microcontroller.

## Project Specific Design Requirements (PSDRs)
1. (Hardware) Ability to transfer letter data from the Raspberry Pi to the microcontroller via UART.
2. (Hardware) Ability to send audio data from the microcontroller to the DAC module via SPI.
3. (Hardware) Ability to output converted analog audio data from DAC to speaker through a team-designed amplifier circuit.
4. (Hardware) Ability to display data from the microcontroller to the OLED display via SPI.
5. (Software) Ability to recognize and translate a subset of 24 stationary ASL alphabet signs with a team-designed computer vision algorithm with an accuracy of 70%.

## Usage
- [Final_Sign2Sound](Final_Sign2Sound/) directory contains our project's STM32CubeIDE code that configures the STM32F091RCT6
  - [Final_Sign2Sound.zip](Final_Sign2Sound.zip) is a zip file containing all of the code in the directory
- [RaspberryPi](RaspberryPi/) directory contains our project's Raspberry Pi 4B driver code that configures the Raspberry Pi to execute the program and interface with the STM32
- [CV](CV/) directory contains code for our computer vision algorithm hosted on the Raspberry Pi

## Install
This project uses the following software and libraries:
- STM32CubeIDE
- pySerial
- PiCamera
- scikit-learn
- pickle
- cv2
- numpy
- PIL
- glob

## Contributors
- Team Leader: [Jennifer Gillette](https://github.com/jennagillette)
- Software Engineer: [Caroline Gilbert](https://github.com/carolinecgilbert)
- Systems Engineer: [Rebecca Horwatt](https://github.com/rhorwatt)
- Hardware Engineer: [Kaushik Karthikeyan](https://github.com/kaushikcodes)


