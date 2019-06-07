#include <Arduino.h>
#include <Wire.h>

#define ENDERECO_MOTOR 0x08

#define PINO_ROTACAO A0
#define PINO_TEMP_AGUA A1
#define PINO_TEMP_AMBIENTE A2
#define PINO_TEMP_OLEO A3
#define PINO_INTERRUP_OLEO 2

#define MAX_ANALOG 1023
#define MAX_ROTACAO 6000
#define MAX_TEMP_AGUA 127
#define MAX_TEMP_AMBIENTE 127
#define MAX_TEMP_OLEO 127

#define MIN_ANALOG 0
#define MIN_ROTACAO 0
#define MIN_TEMP_AGUA -40
#define MIN_TEMP_AMBIENTE -40
#define MIN_TEMP_OLEO -40

#pragma pack(1)

struct ecu_motor_t {
  unsigned int rotacao;
  signed char temp_agua;
  signed char temp_ambiente;
  signed char temp_oleo;
  bool interrup_oleo;
} sensores;

void configuraPinos(void);
void configuraI2C(void);
void enviaDadosSensores();
void atualizaRotacao(void);
void atualizaTempAgua(void);
void atualizaTempAmbiente(void);
void atualizaTempOleo(void);
void atualizaInterrupOleo(void);

void setup()
{
  configuraPinos();
  configuraI2C();
}

void loop()
{
  atualizaSensores();
}

void configuraPinos(void)
{
  pinMode(PINO_INTERRUP_OLEO, INPUT_PULLUP);
}

void configuraI2C(void)
{
  Wire.begin(ENDERECO_MOTOR);
  Wire.onRequest(enviaDadosSensores);
}

void enviaDadosSensores()
{
  Wire.write((char*) &sensores, sizeof(ecu_motor_t));
}

void atualizaSensores(void)
{
  atualizaRotacao();
  atualizaTempAgua();
  atualizaTempAmbiente();
  atualizaTempOleo();
  atualizaInterrupOleo();
}

void atualizaRotacao(void)
{
  sensores.rotacao = map(analogRead(PINO_ROTACAO),
                         MIN_ANALOG, MAX_ANALOG,
                         MIN_ROTACAO, MAX_ROTACAO);
}

void atualizaTempAgua(void)
{
  sensores.temp_agua = map(analogRead(PINO_TEMP_AGUA),
                         MIN_ANALOG, MAX_ANALOG,
                         MIN_TEMP_AGUA, MAX_TEMP_AGUA);
}

void atualizaTempAmbiente(void)
{
  sensores.temp_ambiente = map(analogRead(PINO_TEMP_AMBIENTE),
                         MIN_ANALOG, MAX_ANALOG,
                         MIN_TEMP_AMBIENTE, MAX_TEMP_AMBIENTE);
}

void atualizaTempOleo(void)
{
  sensores.temp_oleo = map(analogRead(PINO_TEMP_OLEO),
                         MIN_ANALOG, MAX_ANALOG,
                         MIN_TEMP_OLEO, MAX_TEMP_OLEO);
}

void atualizaInterrupOleo(void)
{
  sensores.interrup_oleo = !digitalRead(PINO_INTERRUP_OLEO);
}
