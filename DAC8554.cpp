/*
  DAC8554.h - Library for controlling the Digital-
  Analog-Converter on the ArduinoEvaluationBoard.
  Written by Lennart Sobirey, February 2017.
*/

#include "Arduino.h"
#include "DAC8554.h"
#include "SPI.h" 

DAC8554::DAC8554(bool useHardwareSPI)
{
  _hardwareSPI = useHardwareSPI;
  // Default Values based on the Arduino Evaluation Board Wiring. Gotta start somewhere.
  _SCLK = 3; // 13 when using SPI
  _SYNC = 2;
  _D = 4; //  11 when using SPI
  _ENA = 7;
  _A1 = 6;
  _A0 = 5;
  _LDAC = 8;
  _initialized = false; // Pins not set yet
  changeChannelBits(0,0,0,1,1,0,1,1); // Set default values for how to adress the channels.
  _headerByte = B00010110;
}

void DAC8554::setPins(uint8_t SCLK, uint8_t SYNC, uint8_t D, uint8_t ENA, uint8_t A1, uint8_t A0, uint8_t LDAC)
{
  _SCLK = SCLK;
  _SYNC = SYNC;
  _D = D;
  _ENA = ENA;
  _A1 = A1;
  _A0 = A0;
  _LDAC = LDAC;
  _initialized = false;
}

void DAC8554::setDAC(uint8_t A)
{
  bitWrite(_headerByte,6,bitRead(A,0));
  bitWrite(_headerByte,7,bitRead(A,1));
}

void DAC8554::initializePins()
{
    if(_hardwareSPI)
    {
      SPI.begin();
      SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE2));
    }
    else
    {
      pinMode(_SCLK,OUTPUT);
      pinMode(_D,OUTPUT);
      digitalWrite(_SCLK,HIGH);
      digitalWrite(_D,HIGH);
    }
    pinMode(_SYNC,OUTPUT);
    if (_ENA != -1) pinMode(_ENA,OUTPUT);
    if (_A0 != -1) pinMode(_A0,OUTPUT);
    if (_A1 != -1) pinMode(_A1,OUTPUT);
    if (_LDAC != -1) pinMode(_LDAC,OUTPUT);
    digitalWrite(_SYNC,HIGH);
    if (_ENA != -1) digitalWrite(_ENA,LOW);
    if (_A0 != -1) digitalWrite(_A0,LOW);
    if (_A1 != -1) digitalWrite(_A1,LOW);
    if (_LDAC != -1) digitalWrite(_LDAC,LOW);
    _initialized = true;
}

void DAC8554::sendHeader(int channel)
{
  if(channel == 0) {
      bitWrite(_headerByte,5,0);
      bitWrite(_headerByte,2,_ChannelABit1);
      bitWrite(_headerByte,1,_ChannelABit2);
    } else if (channel == 1) {
      bitWrite(_headerByte,5,0);
      bitWrite(_headerByte,2,_ChannelBBit1);
      bitWrite(_headerByte,1,_ChannelBBit2);
    } else if (channel == 2) {
      bitWrite(_headerByte,5,0);
      bitWrite(_headerByte,2,_ChannelCBit1);
      bitWrite(_headerByte,1,_ChannelCBit2);
    } else if (channel == 3) {
      bitWrite(_headerByte,5,0);
      bitWrite(_headerByte,2,_ChannelDBit1);
      bitWrite(_headerByte,1,_ChannelDBit2);
    } else if (channel == -1) {
      bitWrite(_headerByte,2,1);
      bitWrite(_headerByte,5,1);
    }

  if(_hardwareSPI) {
    SPI.transfer(_headerByte);
  } else {
    shiftOut(_D, _SCLK, MSBFIRST, _headerByte);
  } 
}

void DAC8554::sendData(unsigned int value)
{
  if(_hardwareSPI)
  {
    SPI.transfer((value >> 8));
    SPI.transfer(value);
  }
  else 
  {
    shiftOut(_D, _SCLK, MSBFIRST, (value >> 8));
    shiftOut(_D, _SCLK, MSBFIRST, value);
  }
}

void DAC8554::updateChannel(int channel, unsigned int value)
{
  if(!_initialized) initializePins();
  digitalWrite(_SYNC,LOW);
  sendHeader(channel);
  sendData(value);
  digitalWrite(_SYNC,HIGH);
}

void DAC8554::updateAllChannels(unsigned int value)
{
  updateChannel(-1,value);
}

void DAC8554::changeChannelBits(uint8_t ChannelABit1, uint8_t ChannelABit2, uint8_t ChannelBBit1, uint8_t ChannelBBit2, uint8_t ChannelCBit1, uint8_t ChannelCBit2, uint8_t ChannelDBit1, uint8_t ChannelDBit2)
{
    _ChannelABit1 = ChannelABit1; 
    _ChannelABit2 = ChannelABit2; 
    _ChannelBBit1 = ChannelBBit1; 
    _ChannelBBit2 = ChannelBBit2; 
    _ChannelCBit1 = ChannelCBit1; 
    _ChannelCBit2 = ChannelCBit2; 
    _ChannelDBit1 = ChannelDBit1; 
    _ChannelDBit2 = ChannelDBit2; 
}