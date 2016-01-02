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

    Copyright 2016 Michele Martinelli
  */

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include "U8glib.h"
#include "rx5808.h"

// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST); // Fast I2C / TWI

#define button_select 2
#define button_mode 3

#define SSP 10 //receiver SPI Select pin

#define rssi_pin A2  //RSSI input

RX5808 rx5808(rssi_pin);

byte data0 = 0;
byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
uint8_t curr_band = 6;

#define SCANNER_MODE 0
#define RECEIVER_MODE 1

uint8_t curr_mode = SCANNER_MODE;
uint8_t curr_channel = 0;
uint32_t curr_freq;

//battery monitor
#define Voltage A3 //Voltage input
float volt;
uint16_t VoltDivider = 10.5;

uint32_t last_irq;
uint8_t change_freq;
uint8_t killall = 0;

// RSSI default raw range
#define RSSI_MIN_VAL 74
#define RSSI_MAX_VAL 300
//calibration
#define EEPROM_ADR_RSSI_MIN_L 2
#define EEPROM_ADR_RSSI_MIN_H 3
#define EEPROM_ADR_RSSI_MAX_L 4
#define EEPROM_ADR_RSSI_MAX_H 5

#define BIN_H 40

#define CHANNEL_MAX 40
#define CHANNEL_MIN 0

uint16_t rssi_min = 1024;
uint16_t rssi_max = 0;
uint16_t scanVec[320];

// Channels with their Mhz Values
const uint16_t channelFreqTable[] PROGMEM = {
  // Channel 1 - 8
  5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725, // Band A
  5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866, // Band B
  5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945, // Band E
  5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880, // Band F / Airwave
  5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917  // Raceband
};

// All Channels of the above List ordered by Mhz
const uint8_t channelList[] PROGMEM = {
  19, 32, 18, 17, 33, 16, 7, 34, 8, 24, 6, 9, 25, 5, 35, 10, 26, 4, 11, 27, 3, 36, 12, 28, 2, 13, 29, 37, 1, 14, 30, 0, 15, 31, 38, 20, 21, 39, 22, 23
};

const uint8_t channelNames[] PROGMEM = {
  0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,
  0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8,
  0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8,
  0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
  0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8
};

void irq_select_handle() {
  if (millis() - last_irq < 200) //debounce
    return;

  if (digitalRead(button_select) == LOW) {
    if (curr_mode == SCANNER_MODE) {
      curr_band = (curr_band + 1) % 7;
    }
  }
  if (curr_mode == RECEIVER_MODE) {
    if (digitalRead(button_select) == HIGH ) {
      if (millis() - last_irq > 700) //long press to reach the next strong cannel
        curr_channel = rx5808.getNext(curr_channel);
      else
        curr_channel = (curr_channel + 1) % CHANNEL_MAX;

      change_freq = 1;
    }
  }
  killall = 1;
  last_irq = millis();
  // }
}

void irq_mode_handle() {
  if (millis() - last_irq < 500) //debounce
    return;

  if (digitalRead(button_mode) == LOW) {
    
    if (curr_mode == RECEIVER_MODE) {
      curr_mode = SCANNER_MODE;
    }else {
        curr_mode = RECEIVER_MODE;
        curr_channel = rx5808.getMaxPos();
        curr_freq = pgm_read_word_near(channelFreqTable + curr_channel);
        rx5808.set_freq(curr_freq);
        change_freq = 1;
      }
  }
  last_irq = millis();
}

void setup() {
  //button init
  pinMode(button_select, INPUT);
  digitalWrite(button_select, HIGH);
  pinMode(button_mode, INPUT);
  digitalWrite(button_mode, HIGH);

  //display init
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  u8g.setFont(u8g_font_6x10);

  splashScr();

  // initialize SPI:
  pinMode (SSP, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  rx5808.init();

  //power on calibration if button pressed
  while (digitalRead(button_select) == LOW || digitalRead(button_mode) == LOW) {
    rx5808.calibration();
  }

  //receiver init
  rx5808.scan(1, BIN_H);
  curr_channel = rx5808.getMaxPos();
  curr_freq = pgm_read_word_near(channelFreqTable + curr_channel);
  rx5808.set_freq(curr_freq);
  change_freq = 0;

  //rock&roll
  attachInterrupt(digitalPinToInterrupt(button_select), irq_select_handle, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button_mode), irq_mode_handle, CHANGE);
}

void loop(void) {
  battery_measure();

  if (curr_mode == SCANNER_MODE) {
    rx5808.scan(1, BIN_H);
  }

  if (curr_mode == RECEIVER_MODE && change_freq) {
    change_freq = 0;
    curr_freq = pgm_read_word_near(channelFreqTable + curr_channel);
    rx5808.set_freq(curr_freq);
  }

  u8g.firstPage();
  do {

    if (curr_mode == RECEIVER_MODE) {
      receiver_draw(curr_channel);
    }
    if (curr_mode == SCANNER_MODE) {
      scanner_draw(curr_band);
    }

  } while ( u8g.nextPage() );
}
