#include <Arduino.h>
#include <TinyGPS++.h>

unsigned int serialNumber = 2;
unsigned int baudrateMonitorSerial = 115200;
unsigned int baudrateInterfaceSerial = 9600;
unsigned int gpioRX = 16;
unsigned int gpioTX = 17;
unsigned int tempoDeLeituraMs = 40000;

unsigned long timestamp_start;

TinyGPSPlus gps;
HardwareSerial Interface_Serial(serialNumber);

typedef struct
{
  float latitude;
  float longitude;
  int horas;
  int minutos;
  int segundos;
} DadosGps;

void setup()
{
  Serial.begin(baudrateMonitorSerial);
  Interface_Serial.begin(baudrateInterfaceSerial, SERIAL_8N1, gpioRX, gpioTX);

  Serial.printf("\n>>> Iniciando Leitura de dados de localização a cada %d segundos:\n", tempoDeLeituraMs / 1000);
}

void loop()
{
  DadosGps dados_gps;
  timestamp_start = millis();
  do
  {
    while (Interface_Serial.available())
      gps.encode(Interface_Serial.read());
  } while ((millis() - timestamp_start) < tempoDeLeituraMs);

  dados_gps.latitude = gps.location.lat();
  dados_gps.longitude = gps.location.lng();
  dados_gps.horas = gps.time.hour();
  dados_gps.minutos = gps.time.minute();
  dados_gps.segundos = gps.time.second();

  Serial.printf("\n------------------------------------------------------------------------\n");
  Serial.printf("Dados lidos pelo sensor GPS: \n");
  Serial.printf("Latitude = %.6f\n", dados_gps.latitude);
  Serial.printf("Longitude = %.6f\n", dados_gps.longitude);
  Serial.printf("Horário (hh:mm:ss) = %02d:%02d:%02d", dados_gps.horas, dados_gps.minutos, dados_gps.segundos);
  Serial.printf("\n------------------------------------------------------------------------\n");
}