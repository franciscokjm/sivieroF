/* Projeto Curto Circuito - ESP32 & DFPlayer: Teste de Conexão */
/* ---- Bibliotecas ---- */

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

HardwareSerial mySoftwareSerial(1); /* UART1 (0), UART2 (1), UART3 (2). */

DFRobotDFPlayerMini myDFPlayer; /* Cria a variável "myDFPlayer" */


void setup()
{
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);  /* velocidade, tipo de comunicação, pinos RX, TX */
  Serial.begin(115200);                                                     /* velocidade */
  Serial.println();
  Serial.println(F("Iniciando módulo DFPlayer ..."));

  if (!myDFPlayer.begin(mySoftwareSerial))
 {  /* Verifica o funcionamento do módulo.  Se não for capaz de identificar o módulo */                                                                
    Serial.println(myDFPlayer.readType(), HEX);
    Serial.println(F("Erro ao iniciar, verifique:"));
    Serial.println(F("1. A conexao do modulo."));
    Serial.println(F("2. Se o SD card foi inserido corretamente."));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online.")); /* Ao identificar o funcionamento do módulo */

  myDFPlayer.setTimeOut(500); /* Ajusta o tempo de comunicação para 500ms */
 
  /* ----Ajuste do Volume---- */
  //myDFPlayer.volume(10);  /* Volume de 0 a 30. */
  //myDFPlayer.volumeUp();  /* Volume Up */
  
  /*----Ajusta o Equalizador---- */
 // myDFPlayer.EQ(0); /*  0 = Normal, 1 = Pop, 2 = Rock, 3 = Jazz, 4 = Classic, 5 = Bass */
     
  
  /*----Define o dispositivo---- */
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  /*----Quantidade de Arquivos---- */
  //Serial.print(F("Numero de arquivos no cartao SD: "));
  //Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));
  

  Serial.println(myDFPlayer.readState()); //read mp3 state

  myDFPlayer.play(1);  /* Le o primeiro arquivo no cartão */
}
void loop()
{
 /* Vazio, para realizar uma única leitura */
}