/* 
Sub-Projeto Sensor de Nível                                      23/06/2026

Pesquisador Responsável: Prof. Fabio Siviero - siviero@usp.br
Desenvolvimento Técnico: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br
Pesquisador: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Ambiente de desenvolvimento: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16
Hardware: ESP-32-C3 mini
          Módulo TP4056CP 
          Módulo Amplificador PAM8403
          Módulo Leitor de cartão microSD
          Módulo Sensor TTP223

Este este subprojeto faz parte dos trabalhos desenvolvidos no âmbito do 
Centro de Pesquisa e Orientação sobre Deficiência Visual (CpodV) financiado pela 
Fundação de Apoio a Pesquisa do Estado de São Paulo e com participação 
da Secretaria de Estado dos Direitos da Pessoa com Deficiência.
*/

#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}