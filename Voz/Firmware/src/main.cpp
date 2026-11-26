#include <WiFi.h>
#include <driver/i2s.h>

const char* ssid = "voz";
const char* password = "";
const char* serverIP = "192.168.1.100"; // IP do PC/Servidor
const int serverPort = 5005;

#define I2S_WS  40
#define I2S_SCK 42
#define I2S_SD  41
#define I2S_PORT I2S_NUM_0

#define SAMPLE_RATE 16000
#define BUFFER_SAMPLES 512

WiFiClient client;
int32_t raw_samples[BUFFER_SAMPLES];
int16_t pcm_samples[BUFFER_SAMPLES];

// Ajuste este valor para controlar o volume (1.0 = original, 2.5 = 2.5x mais alto, 0.5 = metade)
float ganho = 4.0;

void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = BUFFER_SAMPLES,
    .use_apll = true
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setSleep(false);
  Serial.println("\nWi-Fi Conectado!");
  setupI2S();
}

void loop() {
  // Conecta ou reconecta ao servidor TCP (FFmpeg)
  if (!client.connected()) {
    Serial.println("Conectando ao servidor TCP...");
    if (client.connect(serverIP, serverPort)) {
      client.setNoDelay(true); // Envia imediatamente sem atrasar pacotes
      Serial.println("Conectado ao servidor!");
    } else {
      delay(1000);
      return;
    }
  }

  size_t bytes_read = 0;
  i2s_read(I2S_PORT, &raw_samples, sizeof(raw_samples), &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);

 for (int i = 0; i < samples_read; i++) {
    // 1. Converte o frame de 32-bit do INMP441 para a escala padrão de 16-bit
    int32_t base_sample = raw_samples[i] >> 16;
    
    // 2. Aplica o multiplicador de volume fino
    int32_t sample = (int32_t)(base_sample * ganho);
    
    // 3. Proteção contra distorção (limita ao máximo de 16 bits)
    if (sample > 32767) sample = 32767;
    if (sample < -32768) sample = -32768;
    
    pcm_samples[i] = (int16_t)sample;
  }

  // Envia o bloco PCM garantido via TCP
  client.write((uint8_t*)pcm_samples, samples_read * sizeof(int16_t));
}