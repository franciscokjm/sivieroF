/***************************************************
 Program to sensing touch and auto explain from coustom sound.
****************************************************

 Inclusive Education Materials Project

 Responsable Reasercher: Prof. Dr. Fabio Siviero | fsiviero@usp.br
 Firmware develop: Dr. Kelliton J M Francisco | kelliton@usp.br

 Insect Development Biology Laboratory
 Multiusers Laboratory
'
 Department of Cell and Developmental Biology
 Institute of Biomedical Sciences
    
 ****************************************************/

#include "Arduino.h"

const int Sens8Pin = 14;

void setup(){
  pinMode(Sens8Pin,INPUT);
  //digitalWrite(Sens8Pin,HIGH);
  
  Serial.begin(9600);
}

void loop(){
  int valSens8 = digitalRead(Sens8Pin);
  Serial.println(valSens8);
  delay(100);
}