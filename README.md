# MQTT Explorer

## Predmet:  Seminář C++
## Rok: 2020/21
## Autori: Samuel Olekšák (xoleks00), Michal Findra (xfindr00)

---

### Zadanie : MQTT explorer/dashboard
Vytvorte aplikáciu, ktorá sa pripojí ku serveru (MQTT brokeru) a sleduje všetky alebo vybrané správy (podľa tzv. témy (topic)).

### Preklad a inštalácia
Po rozbalení sa zavolá príkaz `make`, ktorý zostaví program.
Príkaz `make clean` odstráni nepotrebné súbory a `make pack` zabalí súbor do zip archívu.

### Testovanie
V archíve sa nachádza priečinok `examples/`, ktorý po preložení príslušným Makefile poskytuje program `send` (použitie `./send [konfiguračný súbor] [počet sekúnd]`), ktorý podľa zadaného konfiguračného súboru (na ukážku je v adresári dostupný `mqtt.cfg`) bude odosielať správy. Na zachytávanie správ slúži program `receive` (použitie `./receive [topic] [počet správ]`), ktorý zachytí na danom topiku požadovaný počet správ a následne ich vypíše.

#### Formát konfiguračného súboru
Každý riadok je vo formáte `[topic] [časový interval medzi správami] [správa] [...]`, pričom jednotlivé časti sú oddelené medzerou. Napr. riadok `xoleksxfindr 20 server_online` odosila na topic `xoleksxfindr` správu `server_online` každých 20 sekúnd. Namiesto správy sa dá použiť `RANDOM` nasledovanými dvomi medzerou oddelenými číslami, napr. `xoleksxfindr/temperature 10 RANDOM 10 30`, ktoré každých 10 sekúnd odošle správu s náhodným číslom od 10 do 30.

### Funkcionalita
Po spustení aplikácie je v základe nastavený server a téma kompatibilné s testovacím skriptom.

### Dasboard 
V pravej časti aplikácie je možné vytvoriť objekty zobrazované v Dashboard. Objekty môžu mať nastavené meno, tému ktorú majú zobraziť. Objekt môže byť typu `Recieve` takže bude zobrazovať prijaté dáta, alebo typu `Send`, ktorý dokáže odosielať zadanú správu. Pri ukončení aplikácie sa vytvorené widgety v Dashboard uložia do JSON súboru a pri otvorení budú znovu načítané. 

### Snapshots
Vľavo v hore v menu je dostupné tlačidlo `Snapshot`, ktoré uloží snímok aktuálneho stavu do priečinku `snapshots/`.

## Chýbajúca funkcionalita
Program nerozlišuje textové a binárne správy (napr. obrázky).
