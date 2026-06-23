/* 
Sub-Projeto Sensor de Nível                                      19/06/2026

Pesquisador Responsável: Prof. Dr. Cleyton Fernandes Ferrarini - cleyton@ufscar.br 
Desenvolvimento Técnico: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br
Pesquisadores: Prof. Fabio Siviero - siviero@usp.br
               Prof. Dra. Patrícia Saltorato - saltorato@ufscar.br
               Ms. Plínio Cesar Marins - plinio@ufscar.br
               Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br

Ambiente de desenvolvimento: Visual Studio Code version 1.96.4 and PlatformIO Core 6.1.16
Hardware: ESP-32-C3 mini
          BMS HW-373
          Amplificador HW-104
          Sensores TTP223, DS18B20

Este este subprojeto faz parte dos trabalhos desenvolvidos no âmbito do 
Centro de Pesquisa e Orientação sobre Deficiência Visual (CpodV) financiado pela 
Fundação de Apoio a Pesquisa do Estado de São Paulo e com participação 
da Secretaria de Estado dos Direitos da Pessoa com Deficiência.
*/

#include <Arduino.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include "coin.h"

//configuracao de pinos da microcontroladora
#define tg_motor 0 //motor vibratorio
#define sig_ttp223 1 //Sensor de toque
//#define aux1_J8 2 //porta auxiliar 1 em J8
//#define aux2_J8 3 //porta auxiliar 2 em J8
//#define aux3_J8 4 //porta auxiliar 3 em J8
#define sda_DS18S20 8 //sensor de temperatura
//#define aux2_J5 9 //porta auxiliar 2 em J5
#define pwm_PAM8403 10 //PWM-DAC
//#define aux3_J5 20 //porta auxiliar 3 em J5
//#define aux4_J5 21 //porta auxiliar 4 em J5


// Configuração do pino PWM DAC
//const int PWM_PIN = 10;      // GPIO 10
const int PWM_CHAN = 0;      // Canal do LEDC (Obrigatório no Core 2.x)
const int PWM_FREQ = 150000; // 150kHz
const int PWM_RES = 8;       // Resolução de 8 bits (0-255)

// Configurações do Áudio em Memória
const int SAMPLE_RATE = 8000; // Frequência de amostragem do áudio conforme audio original
volatile uint32_t sample_ptr = 0;
//const uint32_t audio_length = sizeof(CoinWav);

volatile bool playing = false;

//variaveis de sistema
#define RESISTOR_VAL 1000000 // 1M Ohm; resistor para stepup
int SigLast = 0; 
bool levelAlarm;
float temperatureC;
unsigned long tempoInicio =0;
unsigned long tempoAtual = 0;
unsigned long tempoDecorrido = 0;
int intervalo = 2000;



//inicializacao de bibliotecas
OneWire oneWire(sda_DS18S20);
DallasTemperature sensors(&oneWire);

// Estrutura do Timer clássico
hw_timer_t *timer = NULL;

// Interrupção clássica usando IRAM_ATTR
void IRAM_ATTR onTimer() {
  if (playing) {
    if (sample_ptr < CoinWav_len) {
      ledcWrite(PWM_CHAN, CoinWav[sample_ptr]);
      sample_ptr++;
    } else {
      // O áudio CHEGOU AO FIM!
      playing = false;             // Altera para falso para parar de processar
      ledcWrite(PWM_CHAN, 0);    // Define o PWM para o meio (0x80 = Silêncio) para não chiar
    }
  }
}

void tocarAudio() {
  timerAlarmDisable(timer); // Desativa o alarme temporariamente para evitar conflitos na ISR
  sample_ptr = 0;           // Reseta o ponteiro de leitura de volta para o início
  playing = true;           // Ativa a flag de reprodução
  timerAlarmEnable(timer);  // Reativa o alarme do timer
}

//Configurações de inicializacao
void setup() {
  
  Serial.begin(115200); //Baud da porta de comnicação serial

  //Configuração de portas
  pinMode(tg_motor,OUTPUT);
  pinMode(sig_ttp223,INPUT);
  //pinMode(aux1_J8,OUTPUT);
  //pinMode(aux2_J8,OUTPUT);
  //pinMode(aux3_J8,OUTPUT);
  pinMode(sda_DS18S20,INPUT);
  //pinMode(aux2_J5,OUTPUT);
  //pinMode(pwm_PAM8403,OUTPUT);
  //pinMode(aux3_J5,OUTPUT);
  //pinMode(aux4_J5,OUTPUT);

  // 1. Configuração do PWM (LEDC) - Padrão Core 2.x
  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES); // Configura o canal 0
  ledcAttachPin(pwm_PAM8403, PWM_CHAN);       // Vincula o GPIO 10 ao canal 0

  // 2. Configuração do Timer de Hardware - Padrão Core 2.x
  // Parâmetros: Número do timer (0), Prescaler (80), Contador crescente (true)
  // Como o clock interno é 80MHz, dividir por 80 faz o timer contar de 1 in 1 microsegundo
  timer = timerBegin(0, 80, true);
  
  // Vincula a função de interrupção ao timer
  timerAttachInterrupt(timer, &onTimer, true);
  
  // Define o valor do alarme em microsegundos e ativa o auto-reload (true)
  // Ex: 1.000.000us / 8000Hz = 125 microsegundos por amostra
  timerAlarmWrite(timer, 1000000 / SAMPLE_RATE, true);
  
  // Habilita o alarme do timer
  timerAlarmEnable(timer);
}

//Secao de transdutores
void motor(bool val){
  digitalWrite(tg_motor,val);
  delay(100);
}

//Secao de sensores
void levelSens(){
  levelAlarm = digitalRead(sig_ttp223);
  //Serial.println(levelAlarm);
  if(levelAlarm){ //aciona alerta se o liquido atinge a região da zona sensível
    motor(true);
    tocarAudio();
    delay(60); 
  }
  else{ //desativa o alerta se o liquido está fora da zona sensivel
    motor(false);
    delay(10);
  }

  //Serial.println(levelAlarm);
}

void readSensors(){
  sensors.requestTemperatures(); 
  temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("C");
  tempoInicio = millis();
  do{
    tempoDecorrido = tempoAtual - tempoInicio;
    levelSens();
    } while (tempoDecorrido < intervalo);      
}

void loop() {

  levelSens(); //ATIVAR APÓS TESTES*******************************
  delay(50);
}