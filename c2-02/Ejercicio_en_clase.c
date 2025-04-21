/* Brian Mendez-Eduardo Meriño-Mateo Peña
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "driver/adc.h"

#define UMBRAL_TACTIL 550                  // Umbral de activación del touch
#define TOUCH_PIN TOUCH_PAD_NUM4           // GPIO13
#define INPUT_POTENCIOMETRO ADC2_CHANNEL_0 // GPIO4
#define ADC_VOLT_MAX 3.3f
#define ADC_MAX_RAW 4095.0f                // 12 bits: 2^12 - 1

void configurar_touch_pad()
{
    touch_pad_init();
    touch_pad_config(TOUCH_PIN, 0);  // Configura el pad táctil
    touch_pad_filter_start(10);      // Filtro de suavizado
}

void app_main(void)
{
    char comando[10];
    int read_raw;
    uint16_t valor_tactil;

    configurar_touch_pad();
    adc2_config_channel_atten(INPUT_POTENCIOMETRO, ADC_ATTEN_DB_11);

    printf("Esperando comando '1' + Enter por serial...\n");

    // Espera "1" + Enter por stdin
    while (1) {
        fgets(comando, sizeof(comando), stdin);
        if (strncmp(comando, "1", 2) == 0) {
            printf("Comando recibido. Esperando toque táctil...\n");
            break;
        }
    }

    // Bucle principal
    while (1) {
        // Leer valor táctil actualizado
        touch_pad_read_filtered(TOUCH_PIN, &valor_tactil);

        if (valor_tactil < UMBRAL_TACTIL) {
            // Leer el valor crudo del ADC
            adc2_get_raw(INPUT_POTENCIOMETRO, ADC_WIDTH_BIT_12, &read_raw);

            float porcentaje = (read_raw / ADC_MAX_RAW) * 100.0f;

            printf("ADC valor: %d\tPorcentaje: %.2f%%\n", read_raw, porcentaje);
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Pausa 1 segundo
        } else {
            vTaskDelay(100 / portTICK_PERIOD_MS);   // Espera corta mientras no hay toque
        }
    }
}
