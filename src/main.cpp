#include <Arduino.h>

#define trig 14
#define echo 27

int distancia = 0;

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
  Serial.begin(9600); 
}

void loop()
{

  distancia = 0.032/2 * distanciaUltrasonico(trig,echo);

  Serial.print("Distancia: ");
  Serial.print(distancia );
  Serial.println(" cm");
  delay(1000);
}
