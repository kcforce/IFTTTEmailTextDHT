/*   send email and text via IFTTT
 *   This sketch is combined from ADAfruit DHT sensor and tdicola for dht.h library
 * 
 *   https://learn.adafruit.com/dht/overview
 *   https://gist.github.com/teos0009/acad7d1e54b97f4b2a88
 */

#include "ESP8266WiFi.h"
#include "DHT.h"
#define DHTPIN 5    

// Uncomment whatever DHT sensor type you're using!
#define DHTTYPE DHT11  // DHT 11
//#define DHTTYPE DHT21  // DHT 21
//#define DHTTYPE DHT22  // DHT 22

DHT dht(DHTPIN,DHTTYPE);
#define WEBSITE  "maker.ifttt.com"


const char *  MY_SSID = "ND-guest";
const char *  MY_PWD  = "";


//use these if you change REST Sting below
const char * Event_Name = "---- Your Event Name ----";
//const char * Maker_Api_Key =    "--- Your API Key Part of URL ---";



void setup()
{
  Serial.begin(115200);
  dht.begin();
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);

  while (WiFi.status() != WL_CONNECTED) //not connected,  ...waiting to connect
    {
      delay(1000);
      Serial.print(".");
    }

  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");
}


void loop()
{
  // Wait a few seconds between measurements.
  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F\n");
 Serial.println ((String) Event_Name );


WiFiClient client;

  if (client.connect(WEBSITE, 80))
    {
          
          Serial.println("WiFi Client connected ");

          client.print(String("POST ") +
         "/trigger/---Your Event Name---/with/key/---Your API Part of URL---?value1="
               +                   (String) h
               +      "&value2=" + (String) t
               +      "&value3=" + (String) f
        //       +    "&Value4=" + (String) hic
        //       +    "&Value5=" + (String) hif
        
                 + " HTTP/1.1\r\n"
                 + "Host: " + WEBSITE + "\r\n" 
                 + "Connection: close\r\n\r\n"
                 );
        
       delay(30000);
    } //end client connect

  else Serial.print("couldnt connect to IFTTT\n");  //if client connect failed
 
  client.stop();
}
