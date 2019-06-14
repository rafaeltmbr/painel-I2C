#include <Arduino.h>
#include <Wire.h>

#define ENDERECO_CAMBIO 0x09

#define ENTRADAS_INICIO 2
#define MARCHAS 5
#define ENTRADAS_FINAL (ENTRADAS_INICIO + MARCHAS)

#define ERRO 'E'
#define NEUTRO 'N'
#define RE 'R'

char marcha;

void configuraPinos(void);
void configuraI2C(void);
void enviaDadosSensores(void);
void atualizaMarcha(void);

void setup()
{
  configuraPinos();
  configuraI2C();
}

void loop()
{
  atualizaMarcha();
}

void configuraPinos(void)
{
  for (int i = ENTRADAS_INICIO; i <= ENTRADAS_FINAL; i++)
  	pinMode(i, INPUT_PULLUP);
}

void configuraI2C(void)
{
  Wire.begin(ENDERECO_CAMBIO);
  Wire.onRequest(enviaDadosSensores);
}

void enviaDadosSensores(void)
{
  Wire.write(marcha);
}

void atualizaMarcha(void)
{
  char m = NEUTRO;
  for (int i = ENTRADAS_INICIO; i <= ENTRADAS_FINAL; i++) {
    if (digitalRead(i) == LOW) {
      if (m != NEUTRO) {
        m = ERRO;
        break;
      }    
      m = (i < ENTRADAS_FINAL) ? ('1' + i - ENTRADAS_INICIO) : RE;
    }
  }
  marcha = m;
}
