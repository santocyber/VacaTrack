#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HardwareSerial.h>
#include <Arduino.h>

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




//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
  

void setup()
{
  Serial.begin(115200);
  Serial.println();
  mySerial.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  //ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(15000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
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
 
    while (mySerial.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(mySerial.read()))
        displayInfo();
  }
  Blynk.run();
  timer.run();
}

void displayInfo()
{ 

  if (gps.location.isValid() ) 
  {
    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);      
    delay (5000);
      
    
  }
  

  Serial.println();
}
