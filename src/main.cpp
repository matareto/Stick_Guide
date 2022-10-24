#include <Arduino.h>
// Librerias requeridas
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include "SPIFFS.h"
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//////////////////////////////Conexxión el a base de datos y la ESP32

// Se proporciona l ainformación de la impresión de carga RTDB
#include <addons/RTDBHelper.h>

//Credenciales dela red WiFi
#define WIFI_SSID "Daniel Amado"
#define WIFI_PASSWORD "Windir1184*"

/* Link de la Base de datos */
#define DATABASE_URL "bastonguia-d271d-default-rtdb.firebaseio.com"
#define DATABASE_SECRET "rfT9VhXd6Fxj0E4AEnKOEEGkcYZGQaHXCyZjyLvl"

/* Se define el objeto de la base de datos */
FirebaseData ultrasonico;

/* Se define la auteticación en Firebase */
FirebaseAuth auth;

/* Se define la configuración de Firebase */
FirebaseConfig config;

String ruta = "StickGuide";

unsigned long dataMillis = 0;
int count = 0;


//Se crea el servidor en el puerto 80
AsyncWebServer server(80);

///////////////////////////////////Variables

#define trig 14
#define echo 27
#define buzzer 22
#define luz 13

int distancia = 0;
int estado;

////////////////////////////////////Funcion del sensor ultrasónico

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

//////////////////////////////Configuración del programa, puertos y enlace

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Conectado al WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectado al WiFi..");
  }

  // Muestra la IP local de la ESP32
  Serial.println(WiFi.localIP());

  //Muestra la versión del FireBase
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Se asigna la base de datos y los secretos de la misma */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;


  /* Inicializa la librería con la autenticación y config de Firebase */
  Firebase.begin(DATABASE_URL, DATABASE_SECRET);
  Firebase.reconnectWiFi(true);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false); });

  // Route to load style.css file
  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/estilos.css", "text/css"); });
  server.on("/data/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/data/main.js"); });

  // Start server
  server.begin();

  pinMode(buzzer, OUTPUT);
  pinMode(luz, INPUT_PULLDOWN);
}

/////////////////////// Programa principal

void loop()
{

  if (millis() - dataMillis > 5000)
  {
    dataMillis = millis();
    Serial.printf("Set int... %s\n", Firebase.setInt(ultrasonico, "/test/int", count++) ? "ok" : ultrasonico.errorReason().c_str());
  }

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

  Firebase.setInt(ultrasonico, ruta + "/Distancia ", distancia);
  Firebase.setInt(ultrasonico, ruta + "/Día-Noche ", estado);
}
