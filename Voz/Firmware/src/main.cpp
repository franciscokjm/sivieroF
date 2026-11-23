#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

// Credenciais da Rede Criada pela Labrador (AP #1)
const char* AP_SSID = "Labrador_Audio_Net";
const char* AP_PASS = "SenhaEspAudio123";

// IP da Labrador na Rede Privada e Porta UDP
const char* LABRADOR_IP = "192.168.4.1";
const uint16_t UDP_PORT = 5000;

// Configuração dos Pinos I2S
#define I2S_WS   4
#define I2S_SD   5
#define I2S_SCK  6
#define I2S_PORT I2S_NUM_0

// Buffer de Áudio
#define SAMPLE_RATE 16000
#define BUFFER_LEN  1024
int32_t i2sBuffer[BUFFER_LEN];

WiFiUDP udp;

void setupI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 256,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
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
    setupI2S();

    // Conecta na Rede Wi-Fi com Senha Criada pela Labrador
    WiFi.begin(AP_SSID, AP_PASS);
    Serial.print("Conectando à Labrador...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado à rede da Labrador!");
}

void loop() {
    size_t bytesRead = 0;
    // Lê os dados brutos do INMP441
    esp_err_t result = i2s_read(I2S_PORT, &i2sBuffer, sizeof(i2sBuffer), &bytesRead, portMAX_DELAY);

    if (result == ESP_OK && bytesRead > 0) {
        // Envia os pacotes de áudio via UDP para a Labrador
        udp.beginPacket(LABRADOR_IP, UDP_PORT);
        udp.write((uint8_t*)i2sBuffer, bytesRead);
        udp.endPacket();
    }
}