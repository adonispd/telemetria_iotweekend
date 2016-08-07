#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN D4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "CrazyTechLabs";
const char* password = "#iotweekend";

const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int luminosidade=0;


    void setup() {
      pinMode(D0, OUTPUT);
      pinMode(D1, OUTPUT);
      pinMode(D2, OUTPUT);
      pinMode(D3, OUTPUT);
      pinMode(D4, OUTPUT);

      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      digitalWrite(D4, LOW);

      dht.begin();
      Serial.begin(115200);

       setup_wifi();

       client.setServer(mqtt_server, 1883);
       client.setCallback(callback);

        
    }

 void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if((int)payload[0]== 0){
    digitalWrite(D4, LOW);
  }
  if((int)payload[0]== 1){
    digitalWrite(D4, HIGH);
  }
  
  
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("adonispd_announcement", "ESP8266 Conectado");
      // ... and resubscribe
      client.subscribe("adonispd_subscribe");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

  void readSensors(){
    luminosidade = analogRead(A0);
    int h = (int) dht.readHumidity();
    delay(2000);
    int t = (int) dht.readTemperature();
    sprintf(msg,"Luz: %d   | Temp: %d *C  | Umid: %d %%", luminosidade, t, h);
    
  }
    
     
  void loop() {

      if (!client.connected()) {
        reconnect();
       }
      client.loop();

      

      long now = millis();
      if (now - lastMsg > 5000) {
        lastMsg = now;
        ++value;
        //snprintf (msg, 75, "hello world #%ld", value);
        readSensors();
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("adonispd", msg);
      }
      
    }
