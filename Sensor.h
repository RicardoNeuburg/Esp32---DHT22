#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>
#include <ThingSpeak.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const int DHT_PIN = 13;

WiFiClient client;

unsigned long CHANNEL = 2143264; 
const char* WRITE_API = "3JI5T9JVS7CXTE5P";
const char* server = "api.thingspeak.com";

int led1 = 18;
int led2 = 19;

int medTemperatura = 35;
int medUmidade = 70;

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

DHTesp dhtSensor;

void setup() {

  //Inicializando o sensor, WiFi e o ThingSpeak
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  dhtSensor.getPin();
  delay(10);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Conected!.");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

}

void loop() {
  float temperatura = dhtSensor.getTemperature();
  float umidade = dhtSensor.getHumidity();
  Serial.println("Temperatura: " + String(temperatura) + "°C");
  Serial.println("Umidade: " + String(umidade) + "%");

  if(temperatura > medTemperatura){
    digitalWrite(led1, HIGH);
  }
  else {
    digitalWrite(led1, LOW);
  }

  if(umidade > medUmidade){
    digitalWrite(led2, HIGH);
  }
  else {
    digitalWrite(led2, LOW);
  }

  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, umidade);

  //Escreve no canal do ThingSepeak
  int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
  if(x == 200){
    Serial.println("Atualização feita com sucesso");
    } else {
      Serial.println("Erro HTTP " + String(x));
    }
  delay (timerDelay);
    }
