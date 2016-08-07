



#include "DHT.h"
#include <ESP8266WiFi.h>


#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int luminosidade=0;
char msg[50];

const char* ssid     = "CrazyTechLabs";
const char* password = "#iotweekend";

const char* mqtt_server = "iot.eclipse.org";



    void setup() {
      pinMode(D0, OUTPUT);
      pinMode(D1, OUTPUT);
      pinMode(D2, OUTPUT);
      pinMode(D3, OUTPUT);

      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);

      dht.begin();
      Serial.begin(115200);

      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Wifi Não conectado, tentando novamente.....");
       }
       Serial.println("");
       Serial.print("WiFi connected!!    ");  
       Serial.print("IP address: ");
       Serial.println(WiFi.localIP());

        
    }
     
    void loop() {
      luminosidade = analogRead(A0);
      //Serial.print("Luz: ");
      //Serial.println(luminosidade);


      int h = (int) dht.readHumidity();
      
      //Serial.print("Umidade: ");
      //Serial.print(h);
      //Serial.println(" % ");
      delay(2000);
      
      int t = (int) dht.readTemperature();
      //Serial.print("Temperatura: ");
      //Serial.print(t);
      //Serial.println(" *C ");

      
      sprintf(msg,"Luz: %d   | Temp: %d *C  | Umid: %d \%", luminosidade, t, h);
      Serial.println(msg);

      if(luminosidade <200)
      {
        digitalWrite(D3, HIGH);
        //Serial.println("luminosidade Baixa");
      } else {
        digitalWrite(D3, LOW);
        //Serial.println("luminosidade Alta");
      }


      
     
      digitalWrite(D0, HIGH);
      //Serial.println("LED Vermelho ON");
      delay(1000);
      
      //Serial.println("LED Amarelo ON");
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
      delay(500);

      
      //Serial.println("LED Verde ON");
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      delay(1000);
      
      digitalWrite(D2, LOW);

      // WiFiClient client;
      // const int httpPort = 80;
      // if (!client.connect(host, httpPort)) {
      //    Serial.println("connection failed");
      //    return;
      //}

      
    }
