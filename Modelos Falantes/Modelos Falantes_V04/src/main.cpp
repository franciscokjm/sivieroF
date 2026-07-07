/* 
Nome do Pojeto: Modelos Falantes                                      19/06/2026

Pesquisador Responsável: Prof. Fabio Siviero - siviero@usp.br
Desenvolvimento Técnico: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br
Pesquisadores: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br
Instituição de Origem: ICB - Universidade de São Paulo (USP)

Ambiente de desenvolvimento: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16
Hardware: Microcontroladora ESP-32-C3 mini
          Módulo BMS TP4056CP (HW-373)
          Módulo Amplificador PAM8403 (HW-104)
          Modulo leitor de cartão microSD (HW-125)
          Módulo Sensor de toque TTP223

Este arquivo é um software de código aberto, licenciado sob a Licença GNU.
Veja o arquivo LICENSE na raiz do projeto para mais detalhes.

Este este subprojeto faz parte dos trabalhos desenvolvidos no âmbito do 
Centro de Pesquisa e Orientação sobre Deficiência Visual (CpodV) financiado pela 
Fundação de Apoio a Pesquisa do Estado de São Paulo e com participação 
da Secretaria de Estado dos Direitos da Pessoa com Deficiência.
*/


#include <Arduino.h>
#include <SPI.h>


//#define touchSensor_1 0
//#define touchSensor_2 1
//#define touchSensor_3 2
//#define touchSensor_4 3
//#define sckPin 4 
//#define misoPin 5
//#define mosiPin 6
//#define ssPin 7
//#define auxJ6_pin1 8
//#define auxJ6_pin2 9
//#define pwmDAC_amplif 10
//#define auxJ6_pin3 20
//#define auxJ6_pin4 21


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("MOSI Pin: ");
  //Serial.println(MOSI);
  Serial.print("MISO Pin: ");
  //Serial.println(MISO);
  Serial.print("SCK Pin: ");
  //Serial.println(SCK);
  Serial.print("SS Pin: ");
  //Serial.println(SS);

  delay(100);
}





