/* 
Nome do Projeto: Modelos Falantes                                      19/06/2026

Pesquisador Responsável: Prof. Fabio Siviero - siviero@usp.br
Desenvolvimento Técnico: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br
Pesquisadores: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br
Instituição de Origem: ICB - Universidade de São Paulo (USP)

Ambiente de desenvolvimento: Visual Studio Code version 1.96.4 e PlatformIO Core 6.1.16
Hardware: Microcontroladora ESP-32-C3 mini
          Módulo BMS TP4056CP (HW-373)
          Módulo Amplificador PAM8403 (HW-104)
          Modulo leitor de cartão microSD (HW-125)
          Módulo Sensor de toque TTP223

Este arquivo é um software de código aberto, licenciado sob a Licença GNU.
Veja o arquivo LICENSE na raiz do projeto para mais detalhes.

Este este subprojeto faz parte dos trabalhos desenvolvidos no âmbito do 
Centro de Pesquisa e Orientação sobre Deficiência Visual (CpodV) financiado pela 
Fundação de Apoio a Pesquisa do Estado de São Paulo e com participação 
da Secretaria de Estado dos Direitos da Pessoa com Deficiência.
*/


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


//#define touchSensor_1 0
//#define touchSensor_2 1
//#define touchSensor_3 3
#define sckPin 4 
#define misoPin 5
#define mosiPin 6
#define ssPin 7
//#define touchSensor_4 8
//#define touchSensor_5 9
#define pwmDAC_amplif 10
//#define touchSensor_6 20
//#define touchSensor_7 21

const int PWM_CHAN = 0;      // Canal do LEDC (Obrigatório no Core 2.x)
const int PWM_FREQ = 150000; // 150kHz
const int PWM_RES = 8;       // Resolução de 8 bits (0-255)
const int SAMPLE_RATE = 32000; // Frequência de amostragem do áudio conforme audio original
const int mudo = 0;

// Configuração do Duplo Buffer (512 bytes é o tamanho ideal para o setor do SD)
const int BUFFER_SIZE = 512; 
uint8_t bufferA[BUFFER_SIZE];
uint8_t bufferB[BUFFER_SIZE];

volatile int current_buffer = 0;     // 0 = Lendo do Buffer A, 1 = Lendo do Buffer B
volatile int buffer_ptr = 0;         // Posição atual dentro do buffer ativo
volatile bool bufferA_ready = false; // Indica se o Buffer A está cheio e pronto para tocar
volatile bool bufferB_ready = false; // Indica se o Buffer B está cheio e pronto para tocar
volatile bool playing = false;       // Status da reprodução
volatile float volume = 1;

File audioFile;
hw_timer_t *timer = NULL;

// Interrupção do Timer: APENAS consome a memória RAM
void IRAM_ATTR onTimer() {
  if (playing) {
    uint8_t raw_sample = mudo; // Inicializa com silêncio por segurança

    // Seleciona o byte do buffer ativo
    if (current_buffer == 0) {
      if (bufferA_ready) {
        raw_sample = bufferA[buffer_ptr];
        buffer_ptr++;
        if (buffer_ptr >= BUFFER_SIZE) {
          buffer_ptr = 0;
          current_buffer = 1;
          bufferA_ready = false;
        }
      }
    } 
    else {
      if (bufferB_ready) {
        raw_sample = bufferB[buffer_ptr];
        buffer_ptr++;
        if (buffer_ptr >= BUFFER_SIZE) {
          buffer_ptr = 0;
          current_buffer = 0;
          bufferB_ready = false;
        }
      }
    }

    // --- A MÁGICA DA MODULAÇÃO DE VOLUME AQUI ---
    // 1. Converte para signed (-128 a 127)
    int16_t signed_sample = (int16_t)raw_sample - 128;
    
    // 2. Aplica o volume
    int16_t scaled_sample = signed_sample * volume;
    
    // 3. Volta para unsigned (0 a 255) e garante que fique dentro dos limites
    uint8_t final_sample = constrain(scaled_sample + 128, 0, 255);

    // Envia a amostra modificada para o PWM
    ledcWrite(PWM_CHAN, final_sample);
  }
}


// Funções do "Produtor": Carregam o SD para a RAM
void carregarBufferA() {
  if (audioFile.available()) {
    int lidos = audioFile.read(bufferA, BUFFER_SIZE);
    if (lidos < BUFFER_SIZE) {
      memset(bufferA + lidos, mudo, BUFFER_SIZE - lidos); // Preenche fim do arquivo com silêncio
    }
    bufferA_ready = true;
  } else {
    playing = false;
    audioFile.close();
    ledcWrite(PWM_CHAN, mudo);
    Serial.println("Fim do áudio.");
  }
}

void carregarBufferB() {
  if (audioFile.available()) {
    int lidos = audioFile.read(bufferB, BUFFER_SIZE);
    if (lidos < BUFFER_SIZE) {
      memset(bufferB + lidos, 128, BUFFER_SIZE - lidos);
    }
    bufferB_ready = true;
  } else {
    playing = false;
    audioFile.close();
    ledcWrite(PWM_CHAN, mudo);
    Serial.println("Fim do áudio.");
  }
}

// Função de disparo do áudio
void tocarAudioSD(const char* nomeArquivo) {
  timerAlarmDisable(timer);
  playing = false;
  
  if (audioFile) audioFile.close();
  
  audioFile = SD.open(nomeArquivo);
  if (!audioFile) {
    Serial.println("Erro: Não foi possível abrir o arquivo!");
    timerAlarmEnable(timer);
    return;
  }
// Pula os primeiros 44 bytes que são o cabeçalho descritivo do arquivo .WAV
  audioFile.seek(44);

  // Pré-carrega ambos os buffers antes de ligar o som
  carregarBufferA();
  carregarBufferB();
  
  buffer_ptr = 0;
  current_buffer = 0;
  playing = true;
  
  timerAlarmEnable(timer);
  Serial.print("Tocando: "); Serial.println(nomeArquivo);
}

void setup() {
  Serial.begin(115200); 
  delay(5000);
  SPI.begin(SCK,MISO,MOSI,SS);
  if (SD.begin(SS)){
    Serial.println("SD Ok!!");
    SD.cardSize();
  }
  // 3. Configuração do PWM (Igual ao seu original)
  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
  ledcAttachPin(pwmDAC_amplif, PWM_CHAN);
  ledcWrite(PWM_CHAN, mudo);

  // 4. Configuração do Timer (Igual ao seu original)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000 / SAMPLE_RATE, true);
  timerAlarmEnable(timer);

}

void loop() {
// CRUCIAL: O loop agora gerencia o abastecimento dos buffers em tempo real
  if (playing) {
    if (!bufferA_ready) {
      carregarBufferA(); // Se o buffer A esvaziou na ISR, o loop lê o SD e enche ele
    }
    if (!bufferB_ready) {
      carregarBufferB(); // Se o buffer B esvaziou na ISR, o loop lê o SD e enche ele
    }
  }

  // Monitor Serial para disparar o áudio manualmente
  if (Serial.available() > 0) {
    Serial.println("Esperando cmd 'S' ");
    char c = Serial.read();
    if (c == 's' || c == 'S') {
      //tocarAudioSD("/1.wav"); // Lembrar da barra "/" indicando a raiz do SD
      tocarAudioSD("/2.wav"); // Lembrar da barra "/" indicando a raiz do SD
      Serial.println("tocou plin...");
    }
  }
  
  // Seus sensores analógicos entram aqui, mas evite usar "delay()" longos no loop 
  // para não travar o abastecimento do buffer de áudio!
}

