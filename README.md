# DAC8554
An Arduino-Library for communication with the DAC8554 Digital Analog Converter from Texas Instruments

To include this, the folder containing the .h and the .cpp file should be inside the C:\Users\USERNAME\Documents\Arduino\libraries\ - folder. 

This code was written to be used in combination with the Arduino Boards I designed a while ago. However, it has been written so that it is agnostic of the specific PCB. You can choose to use the Arduino Hardware-Supported SPI, accessible via the ICSP pins or Digital Pins 11 and 13, or to use any other pins. The Hardware-Supported SPI is much faster (writing is sub 100 microseconds), but when speed is not a requirement, the manual SPI communication can make sense due to the flexibility in the digital pins used (writing takes somewhere around a millisecond).

Read the DAC8554.h for a usage example and method header comments. Enjoy!
