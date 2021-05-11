# MQTT Explorer

## Predmet:  Seminář C++
## Rok: 2020/21
## autori: Samuel Olekšák (xoleks00), Michal Findra (xfindr00)

---

### Zadanie : MQTT explorer/dashboard
Vytvorte aplikáciu, ktorá sa pripojí ku serveru (MQTT brokeru) a sleduje všetky alebo vybrané správy (podľa tzv. témy (topic)).

### Preklad a inštalácia
Po rozbalení sa zavolá príkaz `make`, ktorý zostaví program.
Príkaz `make clean` odstráni nepotrebné súbory a `make pack` zabalí súbor do zip archívu.

### Testovanie
V archíve sa nachádza priečinok `examples/`, ktorý obsahuje script `send.c`, ktorý po preložení príslušným Makefile simuluje odosielanie MQTT správ zo serveru na tému s názvom `xoleksxfindr/#`. 

### Funkcionalita
Po spustení aplikácie je v základe nastavený server a téma kompatibilné s testovacím skriptom.

### Dasboard 
V pravej časti aplikácie je možné vytvoriť objekty zobrazované v Dashboard. Objekty môžu mať nastavené meno, tému ktorú majú zobraziť. Objekt môže byť typu `Recieve` takže bude zobrazovať prijaté dáta, alebo typu `Send`, ktorý dokáže odosielať zadanú správu. Pri ukončení aplikácie sa vytvorené widgety v Dashboard uložia do JSON súboru a pri otvorení budú znovu načítané. 

### Snapshots

