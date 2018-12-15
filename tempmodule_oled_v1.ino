#define BLYNK_PRINT Serial
#include <WEMOS_SHT3X.h>
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//oled ekranı ekliyoruz
#include <Wire.h>
#include <SFE_MicroOLED.h>

//oled bağlantılarını tanımlıyoruz
#define PIN_RESET 9  // Connect RST to pin 9 (req. for SPI and I2C)
#define PIN_DC    8  // Connect DC to pin 8 (required for SPI)
#define PIN_CS    10 // Connect CS to pin 10 (required for SPI)
//#define DC_JUMPER 0 // (I2C only) Set to either 0 or 1, matching the value of the DC Jumper

// Declare a MicroOLED object. The parameters include:
// 1 - Reset pin: Any digital pin
// 2 - D/C pin: Any digital pin (SPI mode only)
// 3 - CS pin: Any digital pin (SPI mode only, 10 recommended)
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // Example SPI Declaration
//MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C Declaration


double temp=0;
double hum=0;
double tempson=0;
double settemp=0;
double offset=0;
double pozoffset=0;
double negoffset=0;






SHT3X sht30(0x45);
WidgetBridge bridge1(V4);
WidgetLED led1(V5);
BlynkTimer timer;

char auth[] = "e4c9dc4021434e959f556c11d5cf0a98";
char ssid[] = "wifi adı buraya yazılacak";
char pass[] = "wifi şifresi buraya yazılacak";

BLYNK_WRITE(V9)
{   
settemp = param.asDouble(); // ayarlanan hedef sıcaklığı app den alıyoruz 
Blynk.virtualWrite(V8,settemp); // hedef sıcaklığı app e yazdırıyoruz.

}  
BLYNK_WRITE(V1)
{   
offset = param.asInt(); // sıcaklık düzeltme katsayısını app üzerinden alıyoruz  

}  
BLYNK_WRITE(V2)
{   
pozoffset  = param.asInt(); // pozitif konfor offsetini app den alıyoruz

}  
BLYNK_WRITE(V3)
{   
negoffset = param.asInt(); // negatif konfor offsetini app den alıyoruz.

}



void myTimerEvent()
{
if(sht30.get()==0)
{
  temp=sht30.cTemp;
  hum=sht30.humidity;
  tempson=temp+(offset*0.1);
  Blynk.virtualWrite(V6, tempson);  // sıcaklık değerini app e yazdırıyoruz.
  Blynk.virtualWrite(V7, hum);  // nem değerini app e yazdırıyoruz

  
  
 if(tempson>settemp+(pozoffset*0.1))
 {
  bridge1.digitalWrite(5, LOW);
    led1.off();
    
 }
    
 

 if(tempson<settemp-(negoffset*0.1))
 {
      bridge1.digitalWrite(5, HIGH);
     led1.on();
   }
  } 
}
    

BLYNK_CONNECTED() 
{ 
  bridge1.setAuthToken("08263ed483394e16b4a6f83f5c8114d6"); 
  Blynk.syncAll();
}

void setup()
{

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);

  //oled ekranı başlatıyoruz
  oled.begin();
  
  }

void loop()
{
  
  Blynk.run();
  timer.run();
//oled ekrana sıcaklık vs yazdırıyoruz
  oled.setFontType(1);  // Set the text to medium (6 columns, 3 rows worth of characters).
  oled.setCursor(0, 0);
  oled.print("Oda Sıcaklık: "); // Print a const string
  oled.println(tempson);  // Print an integer
  oled.print("Hedef Sıcaklık: ");  // Print a float
  oled.println(settemp);  // Print a float
  oled.print("Oda Nem: ");  // Print a float
  oled.println(hum);
  oled.display(); // Draw to the screen

  
  
}







