#include <Arduino.h>
#include <Wire.h>

#define ENDERECO_TRASEIRA 0x0A

#define MAX_ANALOG 1023
#define MAX_DISTANCIA_CM 255
#define MAX_TANQUE_L 50

#define MIN_ANALOG 0
#define MIN_DISTANCIA_CM 0
#define MIN_TANQUE_L 0

#define PINO_BOIA A0
#define PINO_DISTANCIA A1

#pragma pack(1)

struct ecu_traseira_t {
  unsigned int boia;
  unsigned char distancia;
} sensores;

void configuraI2C(void);
void enviaDadosSensores(void);
void atualizaSensores(void);
void atualizaBoia(void);
void atualizaDistancia(void);

void setup()
{
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
  sensores.distancia = map(analogRead(PINO_DISTANCIA),
                           MIN_ANALOG, MAX_ANALOG,
                           MIN_DISTANCIA_CM, MAX_DISTANCIA_CM);
}
