/*
This file is part of OLED 5.8ghz Scanner project.

    OLED 5.8ghz Scanner is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OLED 5.8ghz Scanner is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Nome-Programma.  If not, see <http://www.gnu.org/licenses/>.

    Copyright © 2016 Michele Martinelli
  */

#ifndef rx5808_h
#define rx5808_h

#include "Arduino.h"
#include "const.h"

class RX5808
{
  public:
    RX5808(uint16_t pin);
    uint16_t getVal(uint16_t band, uint16_t channel, uint8_t norm);
    uint16_t getMaxPosBand(uint8_t band);
    uint16_t getMinPosBand(uint8_t band);
    uint16_t getMaxPos();
    uint16_t getMinPos();
    void scan(uint16_t norm_min, uint16_t norm_max);
    void init();
    void calibration();
    void setFreq(uint32_t freq);
    uint16_t getNext(uint16_t channel);
    void abortScan();
    uint16_t getRssi(uint16_t channel);
  private:
    void _calibrationScan();
    uint16_t _readRSSI();
    void _wait_rssi();
    uint16_t _rssiPin;
    uint8_t _stop_scan;
    uint16_t scanVec[CHANNEL_MAX];
};

#endif
