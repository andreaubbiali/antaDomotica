# Anta domotica

Il progetto consiste nel progettare un'anta domotica la quale si apre/chiude in base alla presenza di luce diretta oppure tramite input esterni (bottoni o messaggi di rete). 
L'idea di base nasce dal fatto che il mio ufficio al mattino rimane all'ombra mentre, negli orari pomeridiani, il sole punta diretto all'interno rendendomi difficoltoso il lavoro da pc.

L'apertura o chiusura può essere comandata in due modi:
- automatico: lo stato dell'anta viene deciso automaticamente in base alla presenza di luce diretta (valutata dalla fotoresistenza) ed in base all'orario corrente. Quest ultimo può servire nel caso in cui l'utente voglia che da un certo orario serale fino ad un certo orario alla mattina, le ante rimangano chiuse.

- manuale: lo stato dell'anta può essere modificato tramite la pressione dei bottoni fisici collegati direttamente alla board oppure tramite l'invio di messaggi con il protocollo mqtt.

Ogni qualvolta l'anta si muove, viene attivato (in base a se si sta aprendo o chiudendo) il rispettivo sensore di prossimità, necessario per controllare che non ci sia alcun ostacolo nel raggio di movimento. Qualora il sensore rilevasse un ostacolo il movimento verrà bloccato. Questo tipo di controllo non avviene nel primo movimento di assestamento dell'anta, attenzione quindi che l'intero raggio d'azione sia libero per questo istante.

Per rilevare l'arrivo dell'anta a destinazione, sono stati posti due finecorsa (uno per l'apertura ed uno per la chiusura).

È inoltre presente un sensore di temperatura ed umidità, utilizzato per raffinare il calcolo della distanza calcolato dai sensori di prossimità.

## Messaggi mqtt
Di seguito i messaggi mqtt utilizzati:

### Subscription

### Publish

### Librerie

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