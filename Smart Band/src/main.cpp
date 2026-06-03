/* 
Sub-Projeto Modelos Falantes

Responsable Research: Prof. Dr. Fabio Siviero - fsiviero@usp.br 
Techinical Developer: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br

Hardware: ESP-32-C3 mini
IDE: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16

This sub-project is inscluded in the project "Desenvolvimento de Recursos de 
Tecnologia Assistiva para Atividades de Vida Diaria e Ensino de Pessoas com Deficiencia Visual".
Coordinator: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Grant Project for Pro-Reitoria de Pesquisa e Inovacao da Universidade de Sao Paulo.
*/

#include <Arduino.h>
#include "plimSound.h"
#include "OneWire.h"
#include "DallasTemperature.h"


//Pins Settings
#define tg_motor 0
#define sig_ttp223 1
#define aux1_J8 2
#define aux2_J8 3
#define aux3_J8 4
#define sda_DS18S20 8
#define aux2_J5 9
#define pwm_PAM8403 10
#define aux3_J5 20
#define aux4_J5 21

bool levelAlarm;
bool tempAlarm;

OneWire oneWire(sda_DS18S20);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(tg_motor,OUTPUT);
  pinMode(sig_ttp223,INPUT);
  pinMode(aux1_J8,OUTPUT);
  pinMode(aux2_J8,OUTPUT);
  pinMode(aux3_J8,OUTPUT);
  pinMode(aux2_J5,OUTPUT);
  pinMode(aux3_J5,OUTPUT);
  pinMode(aux4_J5,OUTPUT);
  pinMode(pwm_PAM8403,OUTPUT);
}

void tempSens(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  delay(2000);
}

//Seção de transdutores
void motor(int val){
  digitalWrite(tg_motor,val);
  delay(10);
  tempSens();
}

//Seção de sensores
void levelSens(){
  levelAlarm = digitalRead(sig_ttp223);
  if(levelAlarm){motor(1);}
  else{motor(0);}
  Serial.println(levelAlarm);
}

void loop() {
levelSens();
}







