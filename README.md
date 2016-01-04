# OLED 5.8ghz receiver/scanner

This scanner/receiver is aimed (but not limited to) FPV quadcopter-race addicted. 

When many people need to use video channels over 5.8ghz the interferences become a problem, 
not only because of the superposition of two or more transmitters, but also to the harmonics 
of the transmission frequency or the distance between different TXs. So it's useful to have 
a simple indication of what channel (frequency) is free to use.

The goal of this project is to develop a simple "scanner" that measures the RSSI (Received Signal Strength Intensity)
of each 5.8ghz channel, showing free and used channels.
The hardware used in this project is:

* An RC305 receiver with SPI-modified module (see wiki page) (any RTC6715-based receiver, like the RX5808, can be used) 
* An Arduino (I used a PRO mini)
* A cheap OLED 0.96'' display (to show the informations)
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


##Donations:

Thanking all tx and rx that were sacrificed in the course of this project, I have to admit to being constantly looking for funds to carry out the scanner. Hardware or paypal donations are always accepted.

[![paypal](https://www.paypalobjects.com/it_IT/IT/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YKP2PH72RKPRY)