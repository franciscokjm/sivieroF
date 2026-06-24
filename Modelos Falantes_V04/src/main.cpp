/* 
Sub-Projeto Modelos Falantes                                      19/06/2026

Pesquisador Responsável: Prof. Fabio Siviero - siviero@usp.br
Desenvolvimento Técnico: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br
Pesquisadores: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Ambiente de desenvolvimento: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16
Hardware: Microcontroladora ESP-32-C3 mini
          Módulo BMS TP4056CP (HW-373)
          Módulo Amplificador PAM8403 (HW-104)
          Modulo leitor de cartão microSD (HW-125)
          Módulo Sensor de toque TTP223

Este este subprojeto faz parte dos trabalhos desenvolvidos no âmbito do 
Centro de Pesquisa e Orientação sobre Deficiência Visual (CpodV) financiado pela 
Fundação de Apoio a Pesquisa do Estado de São Paulo e com participação 
da Secretaria de Estado dos Direitos da Pessoa com Deficiência.
*/


#include <Arduino.h>
#include "SPI.h"
#include "SD.h"
#include "FS.h"


#define touchSensor_1 0
#define touchSensor_2 1
#define touchSensor_3 2
#define touchSensor_4 3
#define sckPin 4
#define misoPin 5
#define mosiPin 6
#define csPin 7
#define auxJ6_pin1 8
#define auxJ6_pin2 9
#define pwmDAC_amplif 10
#define auxJ6_pin3 20
#define auxJ6_pin4 21

File file;

void setup() {

  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }
  
  // Initialize SPI with custom pinout
  delay(50);
  SPI.begin(sckPin, misoPin, mosiPin, csPin);
  delay(50);

  delay(5000);
  
  // Start SD Card
  if (!SD.begin(csPin)) {
    Serial.println("Card Mount Failed");
    return;
  }
  //SD.exists("/test.txt");
  Serial.println("passei aqui");

  
}

void loop() {
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("Tamanho do Cartao reconhecido: %llu MB\n", cardSize);

  delay(1000);
  // Nothing to do here
}