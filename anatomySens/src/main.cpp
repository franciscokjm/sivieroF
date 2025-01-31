/* 
Sub-Projeto Modelos Falantes

Responsable Research: Prof. Dr. Fábio Siviero - fsiviero@usp.br 
Techinical Developer: Dr. Kelliton José Mendonça Francisco - kelliton@usp.br

Hardware: ESP WROOM 32 and DFPlayer Mini module
IDE: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16

This sub-project is inscluded in the project "Desenvolvimento de Recursos de 
Tecnologia Assistiva para Atividades de Vida Diária e Ensino de Pessoas com Deficiência Visual".
Coordinator: Prof. Dr. Paulo Eduardo Capel Cardoso 

Projeto financiado pela Pró-Reitoria de Pesquisa e Inovação da Universidade de São Paulo.
*/

//included libraries 
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "SPI.h"

//pin to touch sensors
const int pinSens0 = 13; //sensor 0: S0
const int pinSens1 = 12; //sensor 1: S1
const int pinSens2 = 14; //sensor 2: S2
const int pinSens3 = 27; //sensor 3: S3
const int pinSens4 = 26; //sensor 4: S4
const int pinSens5 = 25; //sensor 5: S5
const int pinSens6 = 33; //sensor 6: S6
const int pinSens7 = 32; //sensor 7: S7
const int pinSens8 = 35; //sensor 8: S8
const int pinSens9 = 34; //sensor 9: S9


const int pinSCK = 18; //pin to RF signal at 5kHz

//variables of status to sensors
int stS0, stS1, stS2, stS3, stS4;
int stS5, stS6, stS7, stS8, stS9;


HardwareSerial mySoftwareSerial(1); //UART2 (1)
DFRobotDFPlayerMini myDFPlayer; //create variable "myDFPlayer"


void setup()
{
  pinMode(pinSCK,OUTPUT); //setting pinSCK to out signal
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17); //parameters to Serial Communication between ESP32 and DFPlayer
  Serial.begin(115200); //baud rate to Serial Communication Protocol between ESP32 and PC                       
  SPI.begin(pinSCK); //start RF signal to sensors
  
  Serial.println();
  Serial.println(F("Starting DFPlayer module..."));

  //Check if module is ready
  if (!myDFPlayer.begin(mySoftwareSerial))
  {  
    Serial.println(myDFPlayer.readType(), HEX);
    Serial.println(F("Fail on start, check:"));
    Serial.println(F("1. The module connection."));
    Serial.println(F("2. If was insert the SD card."));
    while (true);
  }
  Serial.println(F("DFPlayer online."));

  myDFPlayer.setTimeOut(500);//Setting the time of commnunicaton (500ms)

  Serial.println(myDFPlayer.readState()); //read mp3 state
  delay(3000); //delay of 3000s
  //myDFPlayer.play(0);  //read file 0: Welcome mesage
}

void loop()
{
  stS0 = analogRead(pinSens0); 
  if (stS0<3500){
   myDFPlayer.play(1);  //read file 1
  } 
  /*
   stS1 = analogRead(pinSens1); 
   if (stS1<3500){
   myDFPlayer.play(2);  //read file 2
  } 
   stS2 = analogRead(pinSens2); 
   if (stS2<3500){
   myDFPlayer.play(3);  //read file 3
  } 
   stS3 = analogRead(pinSens3); 
   if (stS3<3500){
   myDFPlayer.play(4);  //read file 4
  } 
   stS4 = analogRead(pinSens4); 
   if (stS4<3500){
   myDFPlayer.play(5);  //read file 5
  } 
   stS5 = analogRead(pinSens5);
   if (stS5<3500){
   myDFPlayer.play(6);  //read file 6
  } 
   stS6 = analogRead(pinSens6); 
   if (stS6<3500){
   myDFPlayer.play(7);  //read file 7
  } 
   stS7 = analogRead(pinSens7);
   if (stS7<3500){
   myDFPlayer.play(8);  //read file 8
  } 
   stS8 = analogRead(pinSens8);
   if (stS8<3500){
   myDFPlayer.play(9);  //read file 9
  } 
   stS9 = analogRead(pinSens9); 
   if (stS9<3500){
   myDFPlayer.play(10);  //read file 10
  } 
  */ 
 delay(200); //delay of 200s
}