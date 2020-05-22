
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
  #include <DHT.h>
  #include <Wire.h>
  #include <Adafruit_Sensor.h>
#endif

#define DHTPIN 5
#define DHTTYPE    DHT11     // DHT 22 (AM2302)
#define SensorPin A0 

float sensorValue = 0; 
const char* ssid = "Is Is ja Bos Bos";
const char* password = "Suvi1993";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://nappikoiruli.eu";

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 30000;

void setup() {
  Serial.begin(115200);
  pinMode(DHTPIN, OUTPUT);
  pinMode(A0, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP()); // moi moi moi moi moi master
}

void loop() {


  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      sensorValue = analogRead(SensorPin);
      float newT = dht.readTemperature();

      if (isnan(newT)) {
        Serial.println("Failed to read from DHT sensor!");
      }
      delay(20);
      float newH = dht.readHumidity();

      if (isnan(newH)) {
        Serial.println("Failed to read from DHT sensor!");
      }
      Serial.println(newT);
      Serial.println(newH);

      Serial.println("irri: "+String(sensorValue,3)+" temp: "+String(newT,3)+" hum: "+String(newH,3)); 
      // Prepare your HTTP POST request data
      String httpRequestData = "/data/add/humidity/"+String(newH,3)+"/temperature/"+String(newT,3)+"/irrigationlevel/"+String(sensorValue,3);
      http.begin(serverName + httpRequestData);

      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);


      // Send HTTP GET request
      int httpResponseCode = http.GET();


      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

      delay(1000 * 60 * 30);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}