# OLED 5.8ghz receiver/scanner

This scanner/receiver is intended (but not limited to) FPV quadcopter addicted. 
When many people need to use video channels over 5.8ghz the interferences become a problem, 
and it's useful to have a simple indication of what channel is the one with less "problems". 
Actually, problems are due not only to the superposition of two or more transmitters, but 
also to the harmonics of the transmission frequency or the distance between the tx.

The goal of this project is to develop a simple "scanner" that measures the RSSI (Received Signal Strength Intensity) 
of each 5.8ghz channel (I'm referring to the commonly used FPV channels). 
This way it's possible to know what channel is free to use and what channel is used by someone else. 
All you need is:

* An RX5808 SPI-modified module (any RC305 receiver can be modified as well) * An Arduino (I used a PRO mini)
* A cheap OLED 0.96'' display to show the informations
* some electronic components

please refer to the wiki page for all the hardware details, the software manual and all the other relevant information.

##wiki/documentation is work in progress...

##Some (preliminary) pictures...
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/summary_explained.jpg"  />
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/band_explained.jpg"  />
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/receiver_explained.jpg"  />

##Similar project:

* rx5808-pro (https://code.google.com/p/rx5808-pro/)
Deluxe 5.8GHz (rx5808 module) control based on arduino with TV out.
* fpv_receiver_module by bollop80 (https://github.com/dollop80/fpv_receiver_module)
universal FPV receiver module.


##Donations:

Thanking all tx and rx that were sacrificed in the course of this project, I have to admit to being constantly looking for funds to carry out the scanner. Hardware or paypal donations are always accepted.

[![paypal](https://www.paypalobjects.com/it_IT/IT/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YKP2PH72RKPRY)