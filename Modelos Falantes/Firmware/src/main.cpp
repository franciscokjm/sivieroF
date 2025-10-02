/* 
Sub-Projeto Modelos Falantes

Responsable Research: Prof. Dr. Fábio Siviero - fsiviero@usp.br 
Techinical Developer: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br

Hardware: ESP WROOM 32 and DFPlayer Mini module
IDE: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16

This sub-project is inscluded in the project "Desenvolvimento de Recursos de 
Tecnologia Assistiva para Atividades de Vida Diaria e Ensino de Pessoas com Deficiencia Visual".
Coordinator: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Grant Project for Pro-Reitoria de Pesquisa e Inovacao da Universidade de Sao Paulo.
*/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "HardwareSerial.h"

//Touch Sensor pins
int tchPin[8] = {4, 15, 13, 12, 14, 27, 33, 32};

//Variables to scan Touch Sensors, sensibility sensors and sound volume 
int valtch[8] = {};
int track = 0;
boolean flagPlay = true;
byte volPlayer = 30; //Volume to mp3Player
byte setLimtSens = 32; // Sensibility limit to touch sensor

HardwareSerial dfsd(2);
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void readthcSensors();

//Setup of parameters
void setup()
{
  dfsd.begin(9600, SERIAL_8N1,16,17);  // speed, type, RX, TX
  Serial.begin(9600); //Baud rate of Serial communication between the disposictive and the computer
  delay(3000);

  //Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  delay(3000);
  //Serial.flush();
  
  //if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
  if (!myDFPlayer.begin(dfsd)) {  //Use serial to communicate with mp3.
    
    //Serial.println(myDFPlayer.readType(),HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
    //Serial.flush();
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(volPlayer);  //Set volume value. From 0 to 30
  printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  //myDFPlayer.reset();
}

//Main program
void loop()
{
  delay(500);
  //Serial.print("myDFPlayer.available: ");
  //Serial.println(myDFPlayer.available());

  if (myDFPlayer.available()) {
    readthcSensors();
    //Serial.print("flagPlay: ");
    //Serial.println(flagPlay);
    delay(100);
    if (flagPlay == 1){
      myDFPlayer.play(track); //Play track sound
      flagPlay=false;
    }
  }
  else{
    myDFPlayer.reset();
    delay(3000);
  }
}

//Read the sensor and select the track to play
void readthcSensors(){
  for(int i=0; i<8; i++){
    valtch[i] = touchRead(tchPin[i]);
    //Serial.print("Sensor: ");
    //Serial.print(i);
    //Serial.print(" : ");
    byte nSensor = valtch[i];
    //Serial.println(nSensor);
    delayMicroseconds(100);
    
    //Select track to play
    if (nSensor < setLimtSens){
      delayMicroseconds(100);
      track = i+1;
      //Serial.print("Track: ");
      //Serial.println(track);
      flagPlay = true;
      break;
    }
  }
}

//Function to select the message from microSD
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}