/* 
Sub-Projeto Modelos Falantes

Responsable Research: Prof. Dr. Fabio Siviero - fsiviero@usp.br 
Techinical Developer: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br

Hardware: ESP-32-C3 and DFPlayer Mini module
IDE: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16

This sub-project is inscluded in the project "Desenvolvimento de Recursos de 
Tecnologia Assistiva para Atividades de Vida Diaria e Ensino de Pessoas com Deficiencia Visual".
Coordinator: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Grant Project for Pro-Reitoria de Pesquisa e Inovacao da Universidade de Sao Paulo.
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Pins of ESP32C3 _Do not use Pin 2 in ESP32C3 Super mini_
#define sensPin0 0 //Touch sensor 0 from ESP32 pin 0
#define sensPin1 1 //Touch sensor 1 from ESP32 pin 1
#define sensPin3 3 //Touch sensor 3 from ESP32 pin 3
#define sensPin4 4 //Touch sensor 4 from ESP32 pin 4
#define rxPin 20 //Touch sensor 0 from ESP32 pin 0
#define txPin 21 //Touch sensor 0 from ESP32 pin 0

//Limit to trigger of the touch sensor
#define limitSens0 9000
#define limitSens1 9000
#define limitSens3 9000
#define limitSens4 9000
boolean flagStart = true;

//Variables to read value from pin sensors
int valSensPin0, valSensPin1, valSensPin3, valSensPin4;

//Initializing functions from libraries
EspSoftwareSerial::UART FPSerial;
DFRobotDFPlayerMini myDFPlayer;

//Function to measure "capacitive" variations from pin sensors
long measureTouch(int pin) {
  // Discharge cicle
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(0); //1

  //Measure charge time
  long startTime = micros();
  pinMode(pin, INPUT); //Charging
  while (digitalRead(pin) == LOW) { //Wait charging of pin sensor
    if (micros() - startTime > 10000) return 10000; // Timeout
  }
  return micros() - startTime;
}

void setup(){
    delay(0); //
    Serial.begin(9600); //Setup to Serial com port
    Serial.setTimeout(500);
    FPSerial.begin(9600,SWSERIAL_8N1,rxPin,txPin,false); //Setup to UART communication
    delay(10000);

    //if (!myDFPlayer.begin(FPSerial, true,true)) {  //Initializing the communications between mp3 player and ESP32
    if (!myDFPlayer.begin(FPSerial, false)) {  //Initializing the communications between mp3 player and ESP32
        delay(1000); //Waiting the initialize
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card or USB drive!"));
        while(true){
            delay(1000);
            Serial.println("Serial fail");    
            FPSerial.flush();
        }
    }
    else{
        Serial.println("Serial ok");
    }
}

//Main program
void loop(){
    static unsigned long timer = millis();
    if (millis() - timer > 1000) {
        timer = millis();
        int value;
        value = myDFPlayer.readFileCounts(); //read all file counts in SD card
        if (value == -1) {  //Error while Reading.
            Serial.println("value = -1");
        }
        else{ //Successfully get the result and read "capacitive" sensors.
            if(flagStart==true){
                myDFPlayer.play(9); //Play track 9 from SD card
                flagStart = false;
            }
            Serial.println(value);
            valSensPin0 = measureTouch(sensPin0);
            valSensPin1 = measureTouch(sensPin1);
            valSensPin3 = measureTouch(sensPin3);
            valSensPin4 = measureTouch(sensPin4);

            //Tests of value pin sensors and trigger values
            if (valSensPin0 < limitSens0){
                Serial.println("valSensPin0");
                myDFPlayer.play(1); //Play track 1 from SD card
            }
            if (valSensPin1 < limitSens1){
                Serial.println("valSensPin1");
                myDFPlayer.play(2); //Play track 2 from SD card
            }
            if (valSensPin3 < limitSens3){
                Serial.println("valSensPin3");
                myDFPlayer.play(3); //Play track 3 from SD card
            }
            if (valSensPin4 < limitSens4){
                Serial.println("valSensPin4");
                myDFPlayer.play(4); //Play track 4 from SD card
            }
            Serial.println(valSensPin0);
            Serial.println(valSensPin1);
            Serial.println(valSensPin3);
            Serial.println(valSensPin4);
        }
    }
    delay(500); //500
}    