#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define ENDERECO_TRASEIRA 0x0A

#define MAX_ANALOG 1023
#define MAX_DISTANCIA_CM 255
#define MAX_TANQUE_L 50

#define MIN_ANALOG 0
#define MIN_DISTANCIA_CM 0
#define MIN_TANQUE_L 0

#define PINO_BOIA A0
#define PINO_TRIGGER 2
#define PINO_ECHO 3

#define SOM_CM_US 0.0343

#pragma pack(1)

struct ecu_traseira_t {
  unsigned char boia;
  unsigned char distancia;
} sensores;

void configuraSensorUltrasonico(void);
void configuraI2C(void);
void enviaDadosSensores(void);
void atualizaSensores(void);
void atualizaBoia(void);
void atualizaDistancia(void);
void requisitaLeituraUltrassonico(void);

void setup()
{
  configuraSensorUltrassonico();
  configuraI2C();
}

void loop()
{
  atualizaSensores();
}

void configuraI2C(void)
{
  Wire.begin(ENDERECO_TRASEIRA);
  Wire.onRequest(enviaDadosSensores);
}

void configuraSensorUltrassonico(void)
{
  pinMode(PINO_TRIGGER, OUTPUT);
  pinMode(PINO_ECHO, INPUT);  
}

void enviaDadosSensores(void)
{
  Wire.write((char*) &sensores, sizeof(ecu_traseira_t));
}

void atualizaSensores(void)
{
  atualizaBoia();
  atualizaDistancia();
}

void atualizaBoia(void)
{
  sensores.boia = map(analogRead(PINO_BOIA),
                      MIN_ANALOG, MAX_ANALOG,
                      MIN_TANQUE_L, MAX_TANQUE_L);
}

void atualizaDistancia(void)
{
  requisitaLeituraUltrassonico();  
  unsigned long duracao = pulseIn(PINO_ECHO, HIGH);
  unsigned int distancia_cm = round(duracao * SOM_CM_US / 2);
  
  sensores.distancia = distancia_cm < 256 ? distancia_cm : 255;
}

void requisitaLeituraUltrassonico(void)
{
  digitalWrite(PINO_TRIGGER, LOW);
  delayMicroseconds(2);
  
  digitalWrite(PINO_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIGGER, LOW);
}