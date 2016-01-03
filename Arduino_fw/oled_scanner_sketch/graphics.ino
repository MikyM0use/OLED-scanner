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

#define FRAME_START_X 0
#define FRAME_START_Y 7

char buf[80];

void wait_draw() {
  u8g.drawStr(FRAME_START_X, FRAME_START_Y, "please wait... ");
}

void receiver_draw( uint32_t channel) {

  //display voltage
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(FRAME_START_X, FRAME_START_Y, "vbatt: ");
  u8g.setPrintPos(FRAME_START_X + 40, FRAME_START_Y);
  u8g.print(volt);

  //display current freq and the next strong one
  u8g.setFont(u8g_font_8x13);
  sprintf (buf, "curr [%x]%d:%d", pgm_read_byte_near(channelNames + channel), pgm_read_word_near(channelFreqTable + channel), rx5808.getRssi(channel)); //frequency:RSSI
  u8g.drawStr(FRAME_START_X, FRAME_START_Y + 15, buf);

  uint16_t next_chan = rx5808.getNext(channel);
  sprintf (buf, "next [%x]%d:%d", pgm_read_byte_near(channelNames + next_chan), pgm_read_word_near(channelFreqTable + next_chan), rx5808.getRssi(next_chan)); //frequency:RSSI
  u8g.drawStr(FRAME_START_X, FRAME_START_Y + 30, buf);

  //histo below
  for (int i = CHANNEL_MIN; i < CHANNEL_MAX; i++) {
    uint8_t channelIndex = pgm_read_byte_near(channelList + i); //retrive the value based on the freq order
    uint16_t rssi_scaled = map(rx5808.getRssi(channelIndex), 1, BIN_H, 1, BIN_H / 2);
    u8g.drawVLine(5 + 3 * i, 65 - rssi_scaled, rssi_scaled); //for bar plot, half size because of the small space
  }

}

//draw all the channels of a certain band
void scanner_draw(uint8_t band) {
  int i, x = 0, y = 0, offset = 8 * band;
  u8g.setFont(u8g_font_8x13B); //header

  switch (band) {
    case 0:
      u8g.drawStr(0, 10, "BAND A");
      break;
    case 1:
      u8g.drawStr(0, 10, "BAND B");
      break;
    case 2:
      u8g.drawStr(0, 10, "BAND E");
      break;
    case 3:
      u8g.drawStr(0, 10, "FATSHARK");
      break;
    case 4:
      u8g.drawStr(0, 10, "RACEBAND");
      break;
    case 5:
      spectrum_draw();
      return;
    case 6:
      summary_draw();
      return;
  }

  u8g.setFont(u8g_font_6x10); //smaller for channels

  //make a small histo down left and print the rssi information of the 8 channels of the band
  for (i = 0; i < 8; i++) {
    u8g.drawVLine(2 * i + 5, 20 + BIN_H - rx5808.getRssi(offset + i), rx5808.getRssi(offset + i)); //for bar plot
    sprintf (buf, "%d:%d", pgm_read_word_near(channelFreqTable + offset + i), rx5808.getRssi(offset + i)); //frequency:RSSI

    u8g.drawStr(30 + x, 30 + 10 * y++, buf);

    if (i == 3) {
      x = 45;
      y = 0;
    }
  }

  //computation of the max value
  uint16_t chan = rx5808.getMaxPosBand(band);
  sprintf (buf, "MAX %d", pgm_read_word_near(channelFreqTable + chan));
  u8g.drawStr(75, 7, buf);

  //computation of the min value
  chan = rx5808.getMinPosBand(band);
  sprintf (buf, "MIN %d", pgm_read_word_near(channelFreqTable + chan));
  u8g.drawStr(75, 17, buf);
}


//draw based on the frequency order
void spectrum_draw() {
  u8g.setFont(u8g_font_8x13B);
  u8g.drawStr(0, 10, "SPECTRUM");

  for (int i = CHANNEL_MIN; i < CHANNEL_MAX; i++) {
    uint8_t channelIndex = pgm_read_byte_near(channelList + i); //retrive the value based on the freq order
    u8g.drawVLine(5 + 3 * i, 20 + BIN_H - rx5808.getRssi(channelIndex), rx5808.getRssi(channelIndex)); //for bar plot
  }

  //computation of the max value
  u8g.setFont(u8g_font_6x10);
  uint16_t chan = rx5808.getMaxPos();
  sprintf (buf, "M %x:%d", pgm_read_byte_near(channelNames + chan), pgm_read_word_near(channelFreqTable + chan));
  u8g.drawStr(75, 7, buf);

  //computation of the min value
  chan = rx5808.getMinPos();
  sprintf (buf, "m %x:%d", pgm_read_byte_near(channelNames + chan), pgm_read_word_near(channelFreqTable + chan));
  u8g.drawStr(75, 17, buf);
}

//only one screen to show all the channels
void summary_draw() {

  uint8_t i;
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(FRAME_START_X, FRAME_START_Y, "BAND");
  u8g.drawStr(FRAME_START_X + 80, FRAME_START_Y, "FREE CH");

  u8g.setPrintPos(FRAME_START_X + 35, FRAME_START_Y);
  u8g.print(volt);
  u8g.drawStr(FRAME_START_X + 65, FRAME_START_Y, "v");

  u8g.drawStr(FRAME_START_X + 15, FRAME_START_Y + 12, "A");
  u8g.drawStr(FRAME_START_X + 15, FRAME_START_Y + 22, "B");
  u8g.drawStr(FRAME_START_X + 15, FRAME_START_Y + 32, "E");
  u8g.drawStr(FRAME_START_X + 15, FRAME_START_Y + 42, "F");
  u8g.drawStr(FRAME_START_X + 15, FRAME_START_Y + 52, "R");

  u8g.drawLine(25, 0, 25, 60); //start
  u8g.drawLine(76, 0, 76, 60); //end

#define START_BIN FRAME_START_X+29

#define BIN_H_LITTLE 9
#define START_BIN_Y 13

  //computation of the min value
  for (i = 0; i < 5; i++) {
    uint16_t chan = rx5808.getMinPosBand(i);
    sprintf (buf, "%x %d", pgm_read_byte_near(channelNames + chan), pgm_read_word_near(channelFreqTable + chan));
    u8g.drawStr(FRAME_START_X + 80, FRAME_START_Y + 10 * i + 12, buf);
  }

  for (i = 0; i < 8; i++) {
    uint8_t bin = rx5808.getVal(0, i, BIN_H_LITTLE);
    u8g.drawBox(START_BIN + i * 6, FRAME_START_Y + START_BIN_Y - bin, 2, bin);

    bin = rx5808.getVal(1, i, BIN_H_LITTLE);
    u8g.drawBox(START_BIN + i * 6, FRAME_START_Y + START_BIN_Y - bin + 10, 2, bin);

    bin = rx5808.getVal(2, i, BIN_H_LITTLE);
    u8g.drawBox(START_BIN + i * 6, FRAME_START_Y + START_BIN_Y - bin + 20, 2, bin);

    bin = rx5808.getVal(3, i, BIN_H_LITTLE);
    u8g.drawBox(START_BIN + i * 6, FRAME_START_Y + START_BIN_Y - bin + 30, 2, bin);

    bin = rx5808.getVal(4, i, BIN_H_LITTLE);
    u8g.drawBox(START_BIN + i * 6, FRAME_START_Y + START_BIN_Y - bin + 40, 2, bin);
  }
}

//initial spash screen
void splashScr() {
  //u8g.setFontPosTop();
  u8g.setFont(u8g_font_6x10);
  u8g.firstPage();
  do {

    u8g.drawStr(5, 10, "oled 5.8ghz scanner");
    u8g.drawStr(10, 35, "by mikym0use");

  }
  while ( u8g.nextPage() );
}
