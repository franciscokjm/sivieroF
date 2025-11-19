/* 
Sub-Projeto Modelos Falantes

Responsable Research: Prof. Dr. Fábio Siviero - fsiviero@usp.br 
Techinical Developer: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br

Hardware: ESP-32-C3 and DFPlayer Mini module
IDE: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16

This sub-project is inscluded in the project "Desenvolvimento de Recursos de 
Tecnologia Assistiva para Atividades de Vida Diaria e Ensino de Pessoas com Deficiencia Visual".
Coordinator: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Grant Project for Pro-Reitoria de Pesquisa e Inovacao da Universidade de Sao Paulo.
*/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

//Objeto do DFPlayer que usará a o protocolo de comunição UART
DFRobotDFPlayerMini myDFPlayer;

//Pinos reservados para comunicação UART entre a microcontroladora (MCU) e o MP3 Player
const int rxPin = 20;
const int txPin = 21;

//Pinos de conexão do sensores de toque na ESP32-C3
const int touchPIN_0 = GPIO_NUM_0; // GPIO0 
const int touchPIN_1 = GPIO_NUM_1; // GPIO1
const int touchPIN_3 = GPIO_NUM_3; // GPIO3
const int touchPIN_4 = GPIO_NUM_4; // GPIO4

int touchTHRESHOLD_A0_max = 3000; //Sensibilidade de corte do sensor A0
int touchTHRESHOLD_A0_min = 0; //Sensibilidade de corte do sensor A0
int touchTHRESHOLD_A1_max = 200; //Sensibilidade de corte do sensor A1
int touchTHRESHOLD_A1_min = 0; //Sensibilidade de corte do sensor A1
int touchTHRESHOLD_A3_max = 0; //Sensibilidade de corte do sensor A3
int touchTHRESHOLD_A3_min = 0; //Sensibilidade de corte do sensor A3
int touchTHRESHOLD_A4_max = 0; //Sensibilidade de corte do sensor A4
int touchTHRESHOLD_A4_min = 0; //Sensibilidade de corte do sensor A4
int volPlayer = 30; //Volume do MP3 Player de 0 a 30.
int valA0, valA1, valA2, valA3, valA4; //valores lidos pelos sensores de A0 a A4
int delaySens = 100; //Intervalo entre as leituras de casa sensor (ms)
int delayLoop = 500; //Intervalo entre leityras do bloco Loop (ms) default=10
boolean currentlyTouched = false;

//Teste From Gemini
long measureTouch(int pin) {
  // 1. Descarrega o "circuito"
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(1); 

  // 2. Mede o tempo de carregamento
  long startTime = micros();
  pinMode(pin, INPUT); // Permite o carregamento
  while (digitalRead(pin) == LOW) { // Espera o pino subir (se demorar, há mais capacitância)
    if (micros() - startTime > 10000) return 10000; // Timeout
  }
  return micros() - startTime;
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

void setup() {
  Serial.begin(9600); //Comunicação através do terminal do computador        
  delay(100);
  Serial1.begin(9600, SERIAL_8N1, rxPin, txPin); //Comunicação entre a MCU e o MP3 Player.
  delay(5000);

  if (Serial1.available()) { //comunicação UART ativa entre dispositivos
    Serial.print("MP3 myDFPlayer on-line, favor aguardar 5s. ");
    delay(5000);
  }
  
  if (!myDFPlayer.begin(Serial1)) {  //Use serial to communicate with mp3.

    Serial.println(F("MP3 não iniciado corretamente.")); 
    Serial.println(F("Insira o cartão microSD no leitor e reinicie o dispositivo.")); 
    while(true);
    delay(0);
  }
  myDFPlayer.volume(volPlayer);  //Ajuste do volume do módulo MP3
  digitalWrite(touchPIN_0,INPUT_PULLUP);
  digitalWrite(touchPIN_1,INPUT_PULLUP);
}

void loop() {

  //Leitura dos sensores Touch
  valA0 = measureTouch(touchPIN_0);
  Serial.println(valA0);
  if (valA0 < touchTHRESHOLD_A0_max) {
    myDFPlayer.play(1); // Toca o primeiro audio
    Serial.print("Pino_A0: ");
    Serial.println(valA0);
    delay(delaySens); // Delay para evitar toquer sequenciais
  }

  valA1 = measureTouch(touchPIN_1);
  Serial.println(valA1);
  if (valA1 < touchTHRESHOLD_A1_max) {
    myDFPlayer.play(2); // Toca o segundo audio
    Serial.print("Pino_A1: ");
    Serial.println(valA1);
    delay(delaySens); // Delay para evitar toquer sequenciais
  }

  valA3 = measureTouch(touchPIN_3);
   Serial.println(valA3);
  if (valA3 < touchTHRESHOLD_A3_max) {
    //myDFPlayer.play(4); // Toca o quarto audio
    Serial.print("Pino_A3: ");
    Serial.println(valA3);
    delay(delaySens); // Delay para evitar toquer sequenciais
  }

  valA4 = measureTouch(touchPIN_4);
  Serial.println(valA4);
  if (valA4 < touchTHRESHOLD_A4_max) {
    //myDFPlayer.play(5); // Toca o quinto audio
    Serial.print("Pino_A4: ");
    Serial.println(valA4);
    delay(delaySens); // Delay para evitar toquer sequenciais
  }
  Serial.println("*");
  delay(delayLoop);   
}

