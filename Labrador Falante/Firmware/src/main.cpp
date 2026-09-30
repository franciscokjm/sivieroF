#include <Arduino.h>
#include <BluetoothA2DPSource.h>
#include "driver/i2s.h"

// Definição dos pinos I2S para ESP32-S3 e INMP441
#define I2S_WS   42
#define I2S_SD   15
#define I2S_SCK  14
#define I2S_PORT I2S_NUM_0

#define BUFFER_SIZE 512

BluetoothA2DPSource a2dp_source;

// Configuração do driver I2S para o INMP441
void setup_i2s() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // O INMP441 transmite dados em 32 bits
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // Pino L/R no GND = Canal Esquerdo
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
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

    // Inicializa hardware I2S para leitura do INMP441
    setup_i2s();

    // Define o nome da Labrador à qual a ESP32-S3 deve se conectar automaticamente
    a2dp_source.set_auto_reconnect(true);
    a2dp_source.start("Labrador-BT"); 
}

void loop() {
    // Transmite os dados do microfone apenas quando o link Bluetooth estiver estabelecido
    if (a2dp_source.is_connected()) {
        size_t bytes_read = 0;
        int32_t raw_samples[BUFFER_SIZE];
        int16_t pcm_buffer[BUFFER_SIZE * 2]; // Buffer estéreo (L/R) de 16 bits

        // 1. Lê amostras de 32 bits do INMP441 via I2S
        i2s_read(I2S_PORT, (void*)raw_samples, sizeof(raw_samples), &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);

        // 2. Converte de 32 bits (24 bits alinhados) para PCM 16 bits estéreo
        for (int i = 0; i < samples_read; i++) {
            int16_t pcm_16bit = raw_samples[i] >> 14; // Ajuste de escala do áudio

            pcm_buffer[i * 2]     = pcm_16bit; // Canal Esquerdo
            pcm_buffer[i * 2 + 1] = pcm_16bit; // Canal Direito
        }

        // 3. Envia o buffer processado via Bluetooth A2DP
        a2dp_source.write_data((uint8_t*)pcm_buffer, samples_read * 2 * sizeof(int16_t));
    } else {
        delay(10);
    }
}