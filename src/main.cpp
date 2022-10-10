#include <Arduino.h>
// Librerias requeridas
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include "SPIFFS.h"

#define trig 14
#define echo 27
#define buzzer 22
#define luz 13

int distancia = 0;
int estado;

String d;
// Replace with your network credentials
const char *ssid = "Daniel Amado";
const char *password = "Windir1184*";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


long distanciaUltrasonico(int gat, int eco){
  pinMode(gat, OUTPUT);
  digitalWrite(gat, LOW);
  delayMicroseconds(2);
  digitalWrite(gat, HIGH);
  delayMicroseconds(10);
  digitalWrite(gat, LOW);

  pinMode(eco, INPUT);

  return pulseIn(eco, HIGH);
}
void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectado al WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false); });

  // Route to load style.css file
  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/estilos.css", "text/css"); });


  // Start server
  server.begin();

  pinMode(buzzer, OUTPUT);
  pinMode(luz, INPUT_PULLDOWN);
}

void loop()
{

  distancia = 0.032/2 * distanciaUltrasonico(trig,echo);

  Serial.print("Distancia: ");
  Serial.print(distancia );
  Serial.println(" cm");
  delay(500);

  if(distancia <= 10)
  {
    digitalWrite(buzzer, HIGH);
    delay(20);
    digitalWrite(buzzer, LOW);
  }
    
  else{
    digitalWrite(buzzer,LOW);
  }

  estado = digitalRead(luz);


  if(estado == HIGH )
  {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
  }
  else{
    digitalWrite(buzzer,LOW);
  }

}
