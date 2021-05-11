# MQTT Explorer

## Predmet:  Seminář C++
## Rok: 2020/21
## Autori: Samuel Olekšák (xoleks00), Michal Findra (xfindr00)

---

### Zadanie : MQTT explorer/dashboard
Vytvorte aplikáciu, ktorá sa pripojí ku serveru (MQTT brokeru) a sleduje všetky alebo vybrané správy (podľa tzv. témy (topic)).

### Funkcionalita
Po spustení aplikácie je v základe nastavený server a téma kompatibilná s testovacím skriptom. Prichodzie správy sa budú zapisovať do stromovej štruktúry, kde sa dá jednotlivé položky expandovať/schovávať. Na položky v strome sa dá dvakrát kliknúť, následne sa otvorí okno, kde sa zobrazí história ku konkrétnemu topicu.

### Dasboard 
V pravej časti aplikácie je možné vytvoriť objekty zobrazované v Dashboard. Objekty môžu mať nastavené meno, tému ktorú majú zobraziť. Objekt môže byť typu `Recieve` takže bude zobrazovať prijaté dáta, alebo typu `Send`, ktorý dokáže odosielať zadanú správu. Pri ukončení aplikácie sa vytvorené widgety v Dashboard uložia do JSON súboru a pri otvorení budú znovu načítané. Maximálny počet widgetov v aplikácií je 10. 

### Snapshots
V menu aplikácie vľavo hore je možnosť vytvoriť Snapshot konkrétneho stavu, ktorý uloží aktuálny stav aplikácie do `./snapshots`. Snaphot obsahuje poslednú aktuálnu hodnotu všetkých prijatých správ v adresárovej štruktúre zhodnou s tou v aplikácií. 

### Testovanie
V archíve sa nachádza priečinok `examples/`, ktorý po preložení príslušným Makefile poskytuje program `send` (použitie `./send [konfiguračný súbor] [počet sekúnd]`), ktorý podľa zadaného konfiguračného súboru (na ukážku je v adresári dostupný `mqtt.cfg`) bude odosielať správy. Na zachytávanie správ slúži program `receive` (použitie `./receive [topic] [počet správ]`), ktorý zachytí na danom odoberanom topiku požadovaný počet správ a následne ich vypíše. Programy podporuje paramter `--help` s krátkou nápovedou. Testovacie skripty používajú knižnicu `mosquitto`.

#### Formát konfiguračného súboru
Každý riadok je vo formáte `[topic] [časový interval medzi správami] [obsah správy] [...]`, pričom jednotlivé časti sú oddelené medzerou. Napr. riadok `xoleksxfindr 20 server_online` odosiela na topic `xoleksxfindr` správu `server_online` každých 20 sekúnd. Namiesto správy sa dá použiť `RANDOM` nasledovanými dvomi medzerou oddelenými číslami, napr. `xoleksxfindr/temperature 10 RANDOM 10 30`, ktoré každých 10 sekúnd odošle správu s náhodným číslom od 10 do 30.

## Chýbajúca funkcionalita
Program nerozlišuje textové a binárne správy (napr. obrázky), odoslané a prijaté správy nie sú farebne odlíšené, pri vytváraní snapshotu sa nedá zvoliť konkrétny adresár.

## Použité zdroje
(Eclipse Paho dokumentácia)[https://www.eclipse.org/paho/files/cppdoc/] (licencia)[https://www.eclipse.org/legal/epl-v10.html]
(Eclipse Paho ukážkové príklady)[https://github.com/eclipse/paho.mqtt.cpp/tree/master/src/samples] (licencia)[https://www.eclipse.org/legal/epl-v10.html]
(Mosquitto ukážkové príklady)[https://github.com/eclipse/mosquitto/tree/master/examples] (licencia)[https://www.eclipse.org/legal/epl-v10.html]
