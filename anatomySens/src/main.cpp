/* 
Sub-Projeto Modelos Falantes

Responsable Research: Prof. Dr. Fábio Siviero - fsiviero@usp.br 
Techinical Developer: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br

Hardware: ESP WROOM 32 and DFPlayer Mini module
IDE: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16

This sub-project is inscluded in the project "Desenvolvimento de Recursos de 
Tecnologia Assistiva para Atividades de Vida Diaria e Ensino de Pessoas com Deficiencia Visual".
Coordinator: Prof. Dr. Paulo Eduardo Capel Cardoso 

Grant Project for Pro-Reitoria de Pesquisa e Inovacao da Universidade de Sao Paulo.
*/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/10, /*tx =*/11);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

//Touch Sensor pins
#define tch0 GPIO_NUM_4
#define tch1 GPIO_NUM_2
#define tch2 GPIO_NUM_15
#define tch3 GPIO_NUM_13
#define tch4 GPIO_NUM_12
#define tch5 GPIO_NUM_14
#define tch6 GPIO_NUM_27
#define tch7 GPIO_NUM_33
#define tch8 GPIO_NUM_32

//Variables to scan Touch Sensors
int valtch0,valtch1,valtch2,valtch3,valtch4,valtch5,valtch6,valtch7,valtch8;
int valtch[] = {valtch0,valtch1,valtch2,valtch3,valtch4,valtch5,valtch6,valtch7,valtch8};
int tch[] = {tch0,tch1,tch2,tch3,tch4,tch5,tch6,tch7,tch8};

//CRIAR ROTINA DE MENU


DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void selectMusic(int numMusic);
void readtcSensors();

void setup()
{
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
#else
  FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(15);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
  delay(3000);
}

void loop()
{
  static unsigned long timer = millis();



  if (myDFPlayer.available()) {
    readtcSensors();
    //myDFPlayer.play(2); desabilitado para testes
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void readtcSensors(){

  for(int i=0; i<9; i++){
    
    //USAR AS VARIÁVEIS CRIADAS USANDO ARRAY PARA VARRER AS PORTAS DOS SENSORES.

  }

}

//Function to select the music play from DFPlayer Mini
void selectMusic(int numMusic){
  myDFPlayer.play(numMusic);

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

