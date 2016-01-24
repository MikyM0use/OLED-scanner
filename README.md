# OLED 5.8ghz receiver/scanner

This scanner/receiver is aimed (but not limited to) FPV quadcopter race addicted. 

When many people need to use video channels over 5.8ghz the interferences become a problem, 
not only because of the superposition of two or more transmitters, but also to the harmonics 
of the transmission frequency or the distance between different TXs. So it's useful to have 
a simple indication of channels (frequency) current usage.

The goal of this project is to develop a simple "scanner" that measures the RSSI (Received Signal Strength Intensity)
of each 5.8ghz channel, using this information to determine free and used frequencies.
The hardware needed for this project is:

* An RC305 receiver with SPI-modified module (see wiki page) (any RTC6715-based receiver, like the RX5808, can be used) 
* An Arduino (I used a PRO mini)
* A cheap OLED 0.96'' display
* some electronic components

please refer to the wiki page for all the hardware details, the software manual and all the other relevant information:
[Wiki page](https://github.com/MikyM0use/OLED-scanner/wiki) **(work in progress on this...)**

##Some (preliminary) pictures of the "scanner mode"
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/summary_explained.jpg"  />
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/band_explained.jpg"  />

##A picture of the "receiver mode"
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/receiver_explained.jpg"  />

##Similar projects:

* [rx5808-pr](https://code.google.com/p/rx5808-pro/)

Deluxe 5.8GHz (rx5808 module) control based on arduino with TV out.

* [fpv_receiver_module](https://github.com/dollop80/fpv_receiver_module)

universal FPV receiver module by bollop80.

Stages to success:

[Connection diagram](#connection-diagram)

[STEP 1 - RX SPI mod](#step-1---rx-spi-mod)

[STEP2 - Arduino connection](#step2---arduino-connection)

[STEP3 - Voltage divider](#step3---voltage-divider)

[SOFTWARE section](#software-section)

##Connection diagram
hope this image is enough for the moment, sorry!
![rx5808_hack1](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/connection_diagram/connections_b1.jpg)

##STEP 1 - RX SPI mod

On the RC305 the frequency is changed by using a series of dip switch, but it would be useful if we can change the frequency via software... luckly, the RX module is based on the IC RTC6715, which allows us to use an SPI protocol to change the frequency. But Before that, we need to explain to the RTC6715 to get rid of the dip switch. This is accomplished by removing a resistor inside the RX module...

First of all, you need to open the RC305 module, it will be similar to this:

![rx5808_hack1](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/RC305_nocase.jpg)

next, carefully remove the RX module shield
![rx5808_hack2](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/RC305_opened_original.jpg)

next, remove the resistor marked by the arrow
![rx5808_hack3](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/RC305_opened_arrow.jpg)

next, close all again.

##STEP2 - Arduino connection

This is the more creative step... follow the image in the [Connection diagram](Connection diagram) section and connect the Arduino and the RX module. Remember that since Arduino is 5v powered and the logic levels of the RX modules are 3.3v you need a level shifter (maybe better than mine!).

**It's important at this point to desolder the dip switch or at least cut the trace on the back of the board!**

Just to let you get an idea of the result, this is what I've done, but keep in mind that this includes some other not-documented mods (I'm working on the TV out for the next release)... and yes, it's a mess full of trying/retrying/solder/desolder :)


![finished_front](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/finished_front.jpg)
![finished_back](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/finished_back.jpg)

##STEP3 - Voltage divider
This is an option to get the voltage of the main battery (that can be a 2s or 3s lipo). I think that the simplest solution is to solder a couple of resistor directly on the input jack connection...

![voltage_divider](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/voltage_divider.jpg)

##SOFTWARE section
There are basically two modes of functioning, "scanner" and "receiver". In the first you can see the RSSI of each band, the spectrum and a summary of the channels usage, while in the "receiver" mode, the scanner acts like a normal RX with autoscan feature. This is the list of the different screen you have on the scanner mode and on the receiver mode:

![rx5808 scanner](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/screen_list.jpg)


##Donations:

Hardware or paypal donations to carry on the project are always accepted.

[![paypal](https://www.paypalobjects.com/it_IT/IT/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YKP2PH72RKPRY)