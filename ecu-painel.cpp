#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <Wire.h>

#define COLUNAS 16
#define LINHAS 2

#define DELAY_INICIALIZACAO 200
#define TEMP_ALARME_AGUA 110

#define ENDERECO_MOTOR 0x08
#define ENDERECO_CAMBIO 0x09
#define ENDERECO_TRASEIRA 0x0A

// pneu: R13 175/80
#define PI 3.141592654
#define RAIO_POL 13
#define ALTURA_PNEU_MM (175 * 0.8)
#define ARO_MM (RAIO_POL * 25.4)
#define DIAMETRO_MM (ARO_MM + 2 * ALTURA_PNEU_MM)
#define PERIMETRO_KM (DIAMETRO_MM * PI / 1000000.0)

#pragma pack(1)

struct ecu_motor_t {
  unsigned int rotacao;
  signed char temp_agua;
  signed char temp_ambiente;
  signed char temp_oleo;
  bool interrup_oleo;
};

struct ecu_cambio_t {
  signed char marcha;
};

struct ecu_traseira_t {
  unsigned char boia;
  unsigned char distancia;
};

struct sensor_t {
  ecu_motor_t motor;
  ecu_cambio_t cambio;
  ecu_traseira_t traseira;
} sensores;

//fonte: https://estadodeminas.vrum.com.br/fichatecnica/volkswagen/gol/2003/005148-9
float const tabelaMarcha[] = {
  3.17, // ré
  3.90, // 1ª
  2.12, // 2ª
  1.37, // 3ª
  1.03, // 4ª
  0.80, // 5ª
  5.12  // diferencial
};

LiquidCrystal lcd1(7, 6, 2, 3, 4, 5);
LiquidCrystal lcd2(13, 12, 8, 9, 10, 11);

void configuraDisplays(void);
void mensagemInicializacao(void);
void leMotor(void);
void leCambio(void);
void leTraseira(void);
void escreveDisplay1(void);
void escreveDisplay2(void);
void mostraAlarmePressao(void);
void mostraAlarmeAgua(void);
void mostraInfo1(void);
void mostraDistancia(void);
void mostraInfo2(void);
unsigned int calculaVelocidade(void);
float relacaoMarcha(void);

void setup()
{
  configuraDisplays();
  mensagemInicializacao();
  Wire.begin();
}

void loop()
{
  leMotor();
  leCambio();
  leTraseira();
  escreveDisplay1();
  escreveDisplay2();
}

void configuraDisplays(void)
{
  lcd1.begin(COLUNAS,LINHAS);
  lcd2.begin(COLUNAS,LINHAS);
}

void mensagemInicializacao(void)
{
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("   ECU PAINEL");
  lcd1.setCursor(0, 1);
  lcd1.print("    REDE I2C");

  lcd2.clear();
  lcd2.setCursor(5, 0);
  lcd2.print("Equipe:");
  lcd2.setCursor(0, 1);
  lcd2.print("Rafhael    Lucas");
  delay(DELAY_INICIALIZACAO/2);

  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("Murilo   Brendon");
  lcd2.setCursor(0, 1);
  lcd2.print("Michel Gay");
  delay(DELAY_INICIALIZACAO/2);

  lcd1.clear();
  lcd2.clear();
}

void leMotor(void)
{
  Wire.requestFrom(ENDERECO_MOTOR, sizeof(ecu_motor_t));
  char* motor = (char*) &sensores.motor;
  for(int i=0; Wire.available() && i < sizeof(ecu_motor_t); i++)
    motor[i] = Wire.read();
  Wire.endTransmission();
}

void leCambio(void)
{
  Wire.requestFrom(ENDERECO_CAMBIO, sizeof(char) );
  if( Wire.available() )
    sensores.cambio.marcha = Wire.read();
  Wire.endTransmission();  
}


void leTraseira(void)
{
  Wire.requestFrom(ENDERECO_TRASEIRA, sizeof(ecu_traseira_t));
  char* traseira = (char*) &sensores.traseira;
  for(int i=0; Wire.available() && i < sizeof(ecu_traseira_t); i++)
    traseira[i] = Wire.read();
  Wire.endTransmission();
}

void escreveDisplay1(void)
{
  if (sensores.motor.interrup_oleo)
	mostraAlarmeOleo();
  else if (sensores.motor.temp_agua >= TEMP_ALARME_AGUA)
    mostraAlarmeAgua();
  else
    mostraInfo1();
}

void escreveDisplay2(void)
{  
  if (sensores.cambio.marcha == 'R')
    mostraDistancia();
  else
    mostraInfo2();
}

void mostraAlarmeOleo(void)
{
  lcd1.setCursor(0, 0);
  lcd1.print(" Baixa  Pressao ");
  lcd1.setCursor(0, 1);
  lcd1.print("    de  oleo    ");
}

void mostraAlarmeAgua(void)
{
  lcd1.setCursor(0, 0);
  lcd1.print("Alta temperatura");
  lcd1.setCursor(0, 1);
  lcd1.print("da agua do motor");
}

void mostraInfo1(void)
{
  static char linha1[17], linha2[17];
  snprintf(linha1, sizeof(linha1),
           "Agua %3d%cC %c%4d", sensores.motor.temp_agua,
           0xB0, 0x0A, sensores.motor.rotacao);
  snprintf(linha2, sizeof(linha2),
           "Oleo %3d%cC %c RPM", sensores.motor.temp_oleo,
           0xB0, 0x0A);

  lcd1.setCursor(0, 0);
  lcd1.print(linha1);  
  lcd1.setCursor(0, 1);
  lcd1.print(linha2);
}

void mostraDistancia(void)
{  
  static char linha2[17];
  snprintf(linha2, sizeof(linha2),
           "     %3d cm     ", sensores.traseira.distancia);
  lcd2.setCursor(0, 0);
  lcd2.print("   Distancia    ");
  lcd2.setCursor(0, 1);
  lcd2.print(linha2);
}

void mostraInfo2(void)
{ 
  static char linha1[17], linha2[17];
  
  snprintf(linha1, sizeof(linha1),
           "Amb.%3d%cC %c%3d L ", sensores.motor.temp_ambiente,
           0xB0, 0x0A, sensores.traseira.boia);
  snprintf(linha2, sizeof(linha2),
           " %3d km/h %c M %c ", calculaVelocidade(), 0x0A,
           sensores.cambio.marcha);

  lcd2.setCursor(0, 0);
  lcd2.print(linha1);  
  lcd2.setCursor(0, 1);
  lcd2.print(linha2);
}

unsigned int calculaVelocidade(void)
{
  return sensores.motor.rotacao * 60.0 * relacaoMarcha() *
    PERIMETRO_KM;
}

float relacaoMarcha(void)
{
  char m = sensores.cambio.marcha;
  float rel = 0.0;
  
  if ( m >= '1' && m <= '5' )
    rel = tabelaMarcha[m - '0'];
  else if ( m == 'R' )
    rel = tabelaMarcha[0];
  else
    return 0.0;
    
  rel *= tabelaMarcha[6];
  
  return 1.0 / rel;
}
