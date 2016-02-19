# OLED 5.8ghz receiver/scanner

Questo progetto è dedicato (ma non limitato) ai maniaci di FPV racing con quadricotteri/droni.

Quando diverse persone si trovano a volare insieme, utilizzando contemporaneamente diversi
canali per la trasmissione video dai loro mezzi (tipicamente su 5.8ghz), le interferenze 
possono diventare un problema, non solo per l'eccessiva vicinanza delle diverse sorgenti 
video (comunemente chiamate Vtx, Video Trasmittenti in inglese), 
ma anche per l'interferenza dovuta alle armoniche della frequenza utilizzata per trasmettere
il segnale video, che si traduce in un disturbo su canali anche "lontane".

Per questo motivo, è molto utile per un pilota avere un'indicazione dei canali attualmente 
in uso sui mezzi in volo o delle frequenze comunque "disturbate".

L'obiettivo di questo progetto è lo sviluppo di uno "scanner" che attraverso la misura del
RSSI (Received Signal Strength Intensity, una misura dell'intensità del segnale ricevuto) 
su ogni canale 5.8ghz, fornisca informazioni per determinare quali canali siano "liberi" e 
quali invece "occupati" in ogni momento.

I componenti necessari per la realizzazione di questo progetto sono:

* una ricevente RC305 con modifica SPI (vedere istruzioni successive). Qualsiasi modulo basato
su chip RTC6715 dovrebbe andare bene, anche un semplice Boscam RX5808, che però va alimentato a 5v.
NB la ricevente non perderà la funzionalità di "ricevente", anzi potrà essere utilizzata come una 
normale ricevente 40CH con autoscan.
* un Arduino (micro, PRO mini ecc vanno bene)
* un display I2C OLED 0.96''
* alcuni componenti elettronici (resistenze, pulsantini, vedere dopo per dettagli)

Il costo totale per la realizzazione dello scanner/ricevente non dovrebbe superare i 30€.

##Alcune immagini della "modalità scanner"
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/summary_explained.jpg"  />
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/band_explained.jpg"  />

##Un'immagine della "modalità ricevitore"
<img src="https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/receiver_explained.jpg"  />

##Progetti simili:

* [rx5808-pr](https://code.google.com/p/rx5808-pro/)

Modulo rx5808 controllato da Arduino con TV out.

* [fpv_receiver_module](https://github.com/dollop80/fpv_receiver_module)

Ricevitore FPV universale by bollop80.

##Iniziamo costruzione, Connessioni:

Spero che questa immagine sia abbastanza per il momento...

![rx5808_hack1](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/connection_diagram/connections_b1.jpg)

In realtà è possibile usare qualsiasi valore tra 1kohm e 10kohm per le resistenze SPI,
mentre per il partitore resistivo necessario a misurare la tensione della batteria
ho utilizzato una resistenza da 15Kohm e una da 10Kohm, ma qualsiasi divisore 1/3
dovrebbe funzionare ugualmente (vedere la sezione relativa al partitore resistivo).

**NB per portare l'alimentazione all'Arduino e la display OLED ci sono diverse possibilità (vedere le frecce rosse nell'immagine):**
* prendere i 5v e la massa direttamente dai pin del modulo RX come ho fatto io
* utilizzare il regolatore lineare della ricevente RC305

NON utilizzare direttamente l'input (NON regolato) della ricevente RC305 a meno che non
siate consapevoli che state bypassando la parte di circuito che regola l'alimentazione.

##STEP 1 - Modifica RX SPI

Originariamente sulla ricevente RC305 la frequenza su cui si riceve il segnale è cambiata attraverso 
un dip switch, ma sarebbe molto utile poterla variare via software.
Fortunatamente, il modulo RX è basato sull'integrato RTC6715, che permette l'utilizzo
del protocollo SPI per cambiare i parametri di ricezione. Prima di poter procedere in questo
senso, però, bisogna far capire al modulo che non deve più tenere conto dei dip switch, 
rimuovendo una resistenza saldata all'interno del modulo stesso...

prima di tutto, bisogna aprire il modulo RX, che sarà simile a questo:

![rx5808_hack1](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/RC305_nocase.jpg)

poi, rimuovere con delicatezza la copertura in alluminio:
![rx5808_hack2](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/RC305_opened_original.jpg)

a questo punto, è necessario rimuovere la resistenza indicata dalla freccia:
![rx5808_hack3](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/RC305_opened_arrow.jpg)

poi, richiudere tutto.

##STEP2 - Connessioni con Arduino

Questa è la parte più creativa del progetto... seguire le connessioni illustrate nella sezione [Connessioni](Connection diagram)
per collegare l'Arduino con il modulo RX. Ricordate di inserire le resistenze, che funziono da adattatori di livelli, sulle linee SPI.

**è molto importante dissaldare il dip switch o almeno tagliare le tracce sul retro del PCB della scheda**

Solo per darvi un'idea del risultato, questo è quello che ho fatto nel primo prototipo (che include
anche alcune features non documentate su cui sto ancora lavorando).


![finished_front](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/finished_front.jpg)
![finished_back](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/finished_back.jpg)

##STEP3 - Partitore resistivo
Questo è un passaggio opzionale, che permette di monitorare costantemente il voltaggio 
dell'alimentazione, molto utile se utilizzate lo scanner al campo alimentantolo
a batterie (che possono essere 2s o 3s). Ho pensato che la soluzione più semplice
fosse saldare due resistenze SMD (15k e 10k ohm) direttamente sul jack di alimentazione,
ma si possono tranquillamente utilizzare anche resistenze "tradizionali".

![voltage_divider](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/Photos/voltage_divider.jpg)

##Sezione SOFTWARE
Praticamente ci sono solo due modalità di funzionamento, "scanner" e "ricevente". Nella prima
è mostrata l'indicazione del RSSI per ogni banda, lo spettro e il sommario
dell'utilizzo dei vari canali video, mentre nella modalità "ricevente" lo scanner si
comporta come una normale ricevente con la funzione "autoscan", attivabile tenendo premuto 
il pulsante "select". Premendo il pulsante "select" brevemente, invece, si avanzerà di un 
canale. Sullo schermo viene indicata la frequenza corrente e la prossima frequenza su cui
probabilmente si trova una trasmissione video (RSSI sopra ad una certa soglia).

La lista delle differenti schermate (auto esplicative) è riportata di seguito:

![rx5808 scanner](https://dl.dropboxusercontent.com/u/9124521/OLED-scanner/screen_list.jpg)

**NB lo scanner deve essere calibrato prima di essere utilizzato. La procedura di calibrazione
è semplice: accendere una qualsiasi vtx su un qualsiasi canale e metterla a un paio di metri
dallo scanner, poi accendere quest'ultimo tenendo premuto uno qualsiasi dei due pulsanti e continuando a tenerlo
premuto per 5 secondi circa. Sullo schermo verrà riportato il valore min e max di RSSI rilevato,
accertarsi che i valori presentino una differenza di circa 40-50 unità, altrimenti ripetere la procedura**

##Donations:

Hardware or paypal donations to carry on the project are always accepted.

[![paypal](https://www.paypalobjects.com/it_IT/IT/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YKP2PH72RKPRY)
