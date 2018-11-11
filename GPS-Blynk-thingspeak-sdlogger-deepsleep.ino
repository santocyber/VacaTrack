#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HardwareSerial.h>
#include <Arduino.h>
#include "ThingSpeak.h"
#include <NTPClient.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File dataFile;
const int chipSelect = 10; // pin for the SD card logger.
long lat,lon;

const int UpdateInterval = 1 * 60 * 1000000;  // e.g. 15 * 60 * 1000000; for a 15-Min update interval (15-mins x 60-secs * 1000000uS)
RTC_DATA_ATTR int bootCount = 0;
int GREEN_LED_PIN = 13;


unsigned long myChannelNumber = 623666;
const char * myWriteAPIKey = "KO2GCBF0RJT4WM1C";
WiFiClient  client;

const int BAUD_RATE = 9600;
HardwareSerial mySerial(2); 

//static const int RXPin = 5, TXPin = 4;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800
//Pino serial RX que deve ser conectado no TX do SIM808, usado para configurar a variável "mySerial"
const int RX_PIN = 16;
//Pino serial TX que deve ser conectado no RX do SIM808, usado para configurar a variável "mySerial"
const int TX_PIN = 17;


TinyGPSPlus gps; // The TinyGPS++ object
WidgetMap myMap(V0);  // V0 for virtual pin of Map Widget

//SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

BlynkTimer timer;

float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS


char auth[] = "922ea8d166054798a4b0bdb5aa524c67";              //Your Project authentication key
char ssid[] = "internetSA";                                       // Name of your network (HotSpot or Router name)
char pass[] = "fadababaca";                                      // Corresponding Password

int16_t utc = -3;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc*3600, 60000);

//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
  

void setup()
{
  Serial.begin(115200);
  Serial.println();
  mySerial.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  //ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  ThingSpeak.begin(client);
    timer.setInterval(15000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
    pinMode(GREEN_LED_PIN,OUTPUT);
     timeClient.begin();
     timeClient.update();
}


void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}


void loop()
{
    digitalWrite(GREEN_LED_PIN,HIGH);
    
    while (mySerial.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(mySerial.read()))
        displayInfo();
  }
  Blynk.run();
  timer.run();
//setup SD card
   Serial.print("Initializing SD card...");

  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  //write down the date (year / month / day         prints only the start, so if the logger runs for sevenal days you only findt the start back at the begin.
   
    dataFile = SD.open("gpsLOG.txt", FILE_WRITE);
    dataFile.print("Start logging on: ");
    dataFile.print(timeClient.getFormattedTime());
    dataFile.println(" ");
    dataFile.println("Latitude              Longitude              Time");
    dataFile.close();
  
}

void displayInfo()
{ 

  
  if (gps.location.isValid() ) 
  {
    
    float lat = (gps.location.lat());     //Storing the Lat. and Lon. 
    float lon = (gps.location.lng()); 



    
    Serial.print("LAT:  ");
    Serial.println(lat, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(lon, 6);
    Blynk.virtualWrite(V1, String(lat, 6));   
    Blynk.virtualWrite(V2, String(lon, 6));  
    myMap.location(move_index, lat, lon, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);


//########### ThingSpeak
    String latbuf;
    latbuf += (String(lat, 6));
    Serial.println(latbuf);
 
    String lonbuf;
    lonbuf += (String(lon, 6));
    Serial.println(lonbuf);
    
    ThingSpeak.setField(1, latbuf);
    ThingSpeak.setField(2, lonbuf);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

//##############################Dataloger sd
    Serial.println(timeClient.getFormattedTime());
        Serial.print("Position: ");
        Serial.print("coordinaat ");Serial.print(lat); Serial.print(".");Serial.print(lat);Serial.print(" ");// print latitude to serialmonitor
        Serial.print(", ");Serial.print(lon); Serial.print(".");Serial.println(lon);// print longitude to serialmonitor
         dataFile = SD.open("gpsLOG.txt", FILE_WRITE);
         if (dataFile) {
    
    dataFile.print(lat); dataFile.print("."); dataFile.print(lat); dataFile.print(" ");// print latitude to the SD card
    dataFile.print("            ");
    dataFile.print(lon); dataFile.print("."); dataFile.print(lon);// print longitude to SD Card
    
   
    dataFile.print("              ");
    
   
    dataFile.println(timeClient.getFormattedTime());
   
    dataFile.close();
    // print to the serial port too:
    Serial.println("data stored");
  }
  ///Serial.println("minute past");
 // }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening gpslog.txt");
  }

      
                      
    delay (10000);
//############DeepSLeep
   Serial.print("Entrando em deep");
   digitalWrite(GREEN_LED_PIN,LOW);
   esp_sleep_enable_timer_wakeup(600000);
  //esp_deep_sleep_enable_timer_wakeup(600000);
  Serial.println("Going to sleep now...");
  esp_deep_sleep_start();
  delay (600000);
     
  }
  
  

  Serial.println();
  Serial.print("Fim");

  
}
