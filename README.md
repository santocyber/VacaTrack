# VacaTrack
Rastreamento de animais utilizando rede WiFi-mesh , ESP32 , modulo GPS , painel solar e bateria 18650 
=============================================

Material Necessario
-------------------

- Modulo GPS neo6mv2

https://www.aliexpress.com/item/2pcs-lot-GY-NEO6MV2-new-GPS-module-with-Flight-Control-Flight-Control-EEPROM-MWC-APM2-5/1811853522.html?spm=2114.search0104.3.2.3f0b3941mAC4IY&ws_ab_test=searchweb0_0,searchweb201602_5_10065_10068_10547_319_5730915_317_10548_10696_5728811_453_10084_454_10083_10618_10307_537_536_5729815_5733215_5733315_328_10059_10884_5731015_5733115_10887_100031_5733415_321_5730015_322_10103_5733515_5733615-5733215,searchweb201603_2,ppcSwitch_0&algo_expid=bf4bd8cb-71c6-4e7d-b8ba-0d2d76123687-0&algo_pvid=bf4bd8cb-71c6-4e7d-b8ba-0d2d76123687

- ESP32 com bateria 18650

https://www.aliexpress.com/item/ESP32-ESP-32S-For-WeMos-WiFi-Wireless-Bluetooth-Development-Board-CP2102-CP2104-Module-With-18650-lithium/32840831496.html?spm=2114.search0104.3.88.5cfc3fbbwQ5FEL&ws_ab_test=searchweb0_0,searchweb201602_5_10065_10068_10547_319_5730915_317_10548_10696_5728811_453_10084_454_10083_10618_10307_537_536_5729815_5733215_5733315_328_10059_10884_5731015_5733115_10887_100031_5733415_321_5730015_322_10103_5733515_5733615,searchweb201603_2,ppcSwitch_0&algo_expid=417e3835-0cf5-4207-a942-0f5a996f2a55-12&algo_pvid=417e3835-0cf5-4207-a942-0f5a996f2a55

- Fotocelula Solar

https://www.aliexpress.com/item/Smart-Thin-Film-Flexible-Solar-Panel-Battery-Charger-with-USB-Cable-2V-0-5Wfor-Power-Bank/32762016979.html?spm=a2g0s.13010208.99999999.260.679e3c00Xu7GQy


- GPS Vizualizer

http://www.gpsvisualizer.com/

https://github.com/Grafana-Track-Map/grafana-track-map

wget https://github.com/tkurki/grafana-track-map/archive/master.zip

Libraries necessarias
----------------
https://github.com/jdollar/espsoftwareserial/

https://github.com/blynkkk/blynk-library

https://github.com/jfturcot/SimpleTimer.git

https://github.com/mathworks/thingspeak-arduino.git

Configurando o Server
---------------------

git clone https://github.com/blynkkk/blynk-server.git

Adicione na source.list

sudo pico /etc/apt/sources.list.d/java-10-debian.list

deb http://ppa.launchpad.net/linuxuprising/java/ubuntu bionic main

deb-src http://ppa.launchpad.net/linuxuprising/java/ubuntu bionic main

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys EEA14886

sudo apt-get update

sudo apt-get install oracle-java10-installer

sudo apt-get install oracle-java10-set-default

wget "https://github.com/blynkkk/blynk-server/releases/download/v0.40.2/server-0.40.2-java8.jar"

java -jar server-0.40.2-java8.jar -dataFolder /home/pi/Blynk




Como vai funcionar
---------------------

https://www.youtube.com/watch?v=-MJ5GVy8dh4


Bibliografia/Videografia
----------------------------

https://github.com/cristiansteib/GPS-neo-6m

https://github.com/cspiel1/esp32-gpstracker

https://github.com/Bjoerns-TB/ttn-gps-tracker

https://www.thethingsnetwork.org/labs/story/gps-tracker

https://github.com/techiesms/Geolocation

https://www.youtube.com/watch?v=F4tMvfXDSbU

https://matchx.io/

Battery life calculator:  http://www.of-things.de/battery-life-calculator.php


https://github.com/santocyber/VacaTrack/blob/master/ESP32-DOIT-DEVKIT-V1-Board-Pinout-36-GPIOs-Copy-768x554.jpg
