/*
  DAC8554.h - Library for controlling the DAC8554 Digital-
  Analog-Converter from Texas Instruments. The Datasheet can
  be found here: http://www.ti.com/lit/ds/symlink/dac8554.pdf
  Written by Lennart Sobirey / Aeolitus, February 2017.

  USAGE:
  #include <DAC8554.h>
  // Create the DAC Object, configured for the 
  // Arduino Uno onboard SPI hardware
  DAC8554 dac = DAC8554(true);
  // Select the DAC with pins A0 and A1 set LOW
  dac.setDAC(0);
  // Set the pins we want to use - 13 and 11 are 
  // fixed because the SPI library is hardwired, 
  // the rest you can set arbitrarily as you want
  dac.setPins(13,2,11,7,6,5,8);
  // Start the SPI connection and set the correct
  // pinmodes and default outputs
  dac.initializePins();
  // Set all channels to output 0V
  dac.updateAllChannels(0);
  // Set Channel B to output 5V
  dac.updateChannel(1,65535);

*/
#ifndef DAC8554_h
#define DAC8554_h

#include "Arduino.h"

class DAC8554
{
  public:
    /*
      Constructor method. 
      params:
        useHardwareSPI - set to true to address the DAC using the hardware SPI pins (D_In is 11, SCLK is 13).
      Pins are set to default values that are somewhat arbitrary - they are based on the default wiring of the
      Arduino Evaluation Board I built a while ago. However, you should use DAC8554::setPins anyways to set 
      the pins you are using. 
     */
    DAC8554(bool useHardwareSPI);
    
    /*
      Set a channel to the desired value. If the pins have not been initialized, do it first.
      params: 
        channel - number of the channel to update (0-3 for channels A-D, -1 for all channels)
        value   - new value of the channel €(0,65535)
     */
    void updateChannel(int channel, unsigned int value);
    
    /*
      Update all channels to the desired value
      params:
        value - new value all channels are set to €(0,65535)
     */
    void updateAllChannels(unsigned int value);
    
    /* 
      Set the hardware pins used to communicate with the DAC.
      For communication, only SCLK, SYNC and D are needed. When designing your PCB, if you do not need to set the 
      values of LDAC, A0, A1 and ENA, you can just hardwire them to ground. 
      params:
        PIN NAME - the number of the digital pin this pin of the DAC is connected to. Set to -1 if the pin is hardwired.
                   Pins SYNC, SCLK, D should not be -1. 
     */
    void setPins(uint8_t SCLK, uint8_t SYNC, uint8_t D, uint8_t ENA, uint8_t A1, uint8_t A0, uint8_t LDAC);
    
    /*
      Set the DAC you are currently talking to. Up to four DACs can be connected in parallel. Only the DAC whos 
      A0 and A1 pins correspond to the two bits transferred here will react to the update.
      params:
        A - integer representation of the DAC you wish to adress (0-3)
     */
    void setDAC(uint8_t A);
    
    /*
      Initialize all pins.
      This method prepares the DAC for communication by setting the correct pinmodes and such.
      Call it once before your first transmission. If you dont, the code will call it automatically.
     */
    void initializePins();

    /*
      If you are bad at designing PCBs like me, you may need to change the bits you use to refer to the individual 
      channels. This method allows you to.
      params:
        ChannelABit1 - First bit used to refer to channel A
        ...
     */
    void changeChannelBits(uint8_t ChannelABit1, uint8_t ChannelABit2, uint8_t ChannelBBit1, uint8_t ChannelBBit2, uint8_t ChannelCBit1, uint8_t ChannelCBit2, uint8_t ChannelDBit1, uint8_t ChannelDBit2);

  private:
    /*
      Set the appropriate bits to adress the selected channel and transfer the headerbyte.
      Bits 1 and 2 are the channel. If pin 5 is set, all channels are updated to the same value.
     */
    void sendHeader(int channel);
    
    /*
      Send the data to the DAC. Data is transmitted most significant bit first. 
     */
    void sendData(unsigned int value);

    uint8_t _ChannelABit1; 
    uint8_t _ChannelABit2; 
    uint8_t _ChannelBBit1; 
    uint8_t _ChannelBBit2; 
    uint8_t _ChannelCBit1; 
    uint8_t _ChannelCBit2; 
    uint8_t _ChannelDBit1; 
    uint8_t _ChannelDBit2; 

    bool _hardwareSPI;
    bool _initialized;
    uint8_t _SCLK;
  	uint8_t _SYNC;
  	uint8_t _D;
  	uint8_t _ENA;
  	uint8_t _A1;
  	uint8_t _A0;
  	uint8_t _LDAC;
    byte _headerByte;
};
#endif