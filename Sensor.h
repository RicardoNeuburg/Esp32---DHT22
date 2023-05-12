#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>
#include <ThingSpeak.h>

const char* ssid = "Wokwi-GUEST"; //Nome da rede a qual o dispositivo se conectará
const char* password = ""; //Senha de acesso da rede
const int DHT_PIN = 13; //Pino a qual o sensor DHT enviara os dados para o Esp32 

WiFiClient client;

unsigned long CHANNEL = 2143264; //Número da identificação do canal do ThingSpeak
const char* WRITE_API = "3JI5T9JVS7CXTE5P"; //Chave da API para requisições
const char* server = "api.thingspeak.com"; //URL da página a qual serão realizadas as requisições

int led1 = 18; //Pino a qual a led vermelha esta conectada
int led2 = 19; //Pino a qual a led amarela esta conectada

int medTemperatura = 35; //Valor da temperatura a ser condicionada
int medUmidade = 70; //Valor da umidade a ser condicionada

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

DHTesp dhtSensor;

void setup() {

  //Inicializando o sensor, WiFi e o ThingSpeak
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22); //Configura qual a entrada do pino(DHT_PIN) e qual o modelo do sensor(DHT22)
  pinMode(led1, OUTPUT); //Declara o pino(led1) como saída(OUTPUT)
  pinMode(led2, OUTPUT); //Declara o pino(led2) como saída(OUTPUT)
  delay(10);

  WiFi.begin(ssid, password); //Recebe o nome da rede(ssid), e a senha da rede(password)
  while(WiFi.status() != WL_CONNECTED){ //Enquanto o wifi não estiver conectado(WL_CONNECTED), um "." será escrito
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Conected!.");
  Serial.println(WiFi.localIP()); //Escreve qual o IP utilizado

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

}

void loop() {
  float temperatura = dhtSensor.getTemperature(); //Retorna o valor da temperatura obtida pelo sensor
  float umidade = dhtSensor.getHumidity(); //Retorna o valor da umidade obtida pelo sensor
  Serial.println("Temperatura: " + String(temperatura) + "°C");
  Serial.println("Umidade: " + String(umidade) + "%");

  if(temperatura > medTemperatura){ //Se a temperatura for acima que medTemperatura(35), o led1 será alimentado
    digitalWrite(led1, HIGH);
  }
  else {
    digitalWrite(led1, LOW);
  }

  if(umidade > medUmidade){ //Se a temperatura for acima que medUmidade(70), o led2 será alimentado
    digitalWrite(led2, HIGH);
  }
  else {
    digitalWrite(led2, LOW);
  }

  ThingSpeak.setField(1, temperatura); //Enviara para o "field 1" o valor obtido na variável "temperatura"
  ThingSpeak.setField(2, umidade); //Enviara para o "field 2" o valor obtido na variáveç "umidade"

  int x = ThingSpeak.writeFields(CHANNEL, WRITE_API); //Se o valor retornado pela função writeFields for 200, a atualização foi sucessida, caso contrário, o erro será comunicado em forma de número.
  if(x == 200){
    Serial.println("Atualização feita com sucesso");
    } else {
      Serial.println("Erro HTTP " + String(x));
    }
  delay (timerDelay);
    }
