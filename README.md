# Anta domotica

Il progetto consiste nel progettare un'anta domotica che si apre/chiude in base alla presenza o meno del sole. L'idea di base nasce dal fatto che il mio ufficio al mattino rimane all'ombra mentre nel pomeriggio il sole punta diretto all'interno rendendomi difficoltoso il lavoro al pc. Se il meccanismo, durante il moto di chiusura o apertura, dovesse rilevare un'ostacolo, tenterà di tornare nella posizione precedente. L'apertura o chiusura potrà essere anche comandata tramite messaggi MQTT oppure utilizzando dei pulsanti. Per avere un ulteriore informazione, sarà inoltre presente un sensore di temperatura con un display.

### libraries

DHT library from adafruit
https://github.com/adafruit/DHT-sensor-library

![Fritzing circuit](antaDomotica.jpg)

## DEFAULT

In default the door will close at 22pm and will open at 6am (from this it will remain in automatic mode)


## AUTOMATIC logic:

The purpose is that if I have decided to close the door it must remain closed (usually during the night) while if is automatic and I said to open it, if the photresistence decide to cloe it, the door should be closed.

photoresistence -> close        time -> open   = close
photoresistence -> open         time -> close  = close
photoresistence -> open         time -> open   = open
photoresistence -> close        time -> close  = close

<b> !ATTENTION! </b>
For implementation behaviour close time must be > of the open time.
So for example
```
open-time: 06:00
close-time: 23:00
```