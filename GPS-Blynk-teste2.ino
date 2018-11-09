/* BlynkifyGPSv2.ino for https://community.blynk.cc/t/arduino-or-nodemcu-gps-tracking-system-on-map-widget/15739
  Sketch for physical GPS device connected to a decent ESP
  In map widget set SHOW MY LOCATION to NO unless you also want Smartphone actual location in addition to GPS device + ESP
*/
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps; // The TinyGPS++ object

SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

BlynkTimer timer;

char auth[] = "6075f93a98734b3abddb3c3ba6babf08";
char ssid[] = "MikroTik Home";
char pass[] = "";
char server[] = "192.168.1.104";

//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
bool simulation = false;           // set to true for simulation mode, else false. If IranHack is true this must be false
                                   // simulation location is in Egypt
                                   
bool IranHack = false;             // set to true if GPS is faulty, else false. If simulation is true this must be false

WidgetMap myMap(V5);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass, server);

  // disable GPS device checking for now
  //timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once

  timer.setInterval(30000L, readGPS); // read GPS data every 30s
  readGPS();                          // read GPS data on each reboot after connection to Blynk server
}

void readGPS(){
  long startTime = millis();
  long counter = 0;
  Serial.print("Started reading GPS data at ");  
  Serial.println(startTime); 
   
  if(simulation == true || IranHack == true)
  {
    long loopmax = 100L;  // max 2,147,483,647 or double this if usigned
    while (counter < loopmax)   
    { 
      counter++;
      Serial.print(".");
      delay(20);           // pause 10ms (1s total) 20ms (2s total)
    }
    Serial.println();
    //counter = 0;   // not required as counter is redefined each time readGPS() is called
    displayInfo();  
  }
  else
  {
    while (ss.available() > 0)  // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        displayInfo();
  }
  Serial.print("Finished reading GPS data at ");
  Serial.print(millis());
  Serial.print(" and it took ");
  Serial.print(millis() - startTime);
  Serial.println("ms.");
  Serial.println("-----------------------------------------------------------");
  Serial.println();   
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V10, "GPS ERROR");  // Value Display widget set as PUSH frequency on V10
    Blynk.virtualWrite(V7, "--------");    // Value Display widget set as PUSH frequency on V7 LATITUDE
    Blynk.virtualWrite(V8, "--------");    // Value Display widget set as PUSH frequency on V8 LONGITUDE
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}

void displayInfo()
{ 

  if (gps.location.isValid() || simulation == true || IranHack == true) 
  {
    //move_index++;                        // moving index, maybe use later
    Blynk.virtualWrite(V10, "GPS OK");
    float latitude = (gps.location.lat());  // removed , 6 as it can't be used for creating the float
    float longitude = (gps.location.lng()); // removed , 6 as it can't be used for creating the float

    if(latitude == 0 && longitude == 0 )    // checking to see if the 2 floats variables are correct
    {
      Blynk.virtualWrite(V7, "--------");   
      Blynk.virtualWrite(V8, "--------");
      Blynk.virtualWrite(V10, "0,0");
    }
  
    if(simulation == true)         // location in Egypt
    {
      latitude  = 30.000123;       // also used for debugging
      longitude = 30.000987;       // also used for debugging  
    }
    
    if(IranHack == true)
    {
      latitude  = 32.644824;
      longitude = 51.681746;
    }    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
      
    Blynk.virtualWrite(V7, String(latitude, 6));   
    Blynk.virtualWrite(V8, String(longitude, 6));  

    myMap.location(move_index, latitude, longitude, "GPS_Location");
  }
  else
  {
    Serial.print(F("INVALID"));
    Blynk.virtualWrite(V10, "GPS INVALID");
    Blynk.virtualWrite(V7, "--------");   
    Blynk.virtualWrite(V8, "--------");  
  }

  Serial.println();
}
