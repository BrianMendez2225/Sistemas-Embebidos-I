/* Brian Mendez - Eduardo Meriño - Mateo Peña
1. Desarrolla un sistema de autenticación basado en un patrón táctil.
 2. El usuario debe tocar la secuencia en un pin táctil:
 (a) 3 toques largos.
 (b) 3 toques cortos.
 (c) 3 toques largos.
 3. Luego de hacer esa secuencia, se debe tocar otro pin táctil para validar
 la secuencia.
 4. Imprimir por serial "APROBADO" o "NO APROBADO" si la secuen
cia ingresada es correcta o no.
 5. Por grupo definir el tiempo a su criterio para determinar que es "toque
 largo" y por "toque corto"
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_timer.h"
#include "esp_log.h"

// Definición de pines de los sensores táctiles
#define PIN_SECUENCIA  TOUCH_PAD_NUM7  // GPIO27 para ingresar la secuencia
#define PIN_VALIDACION     TOUCH_PAD_NUM8  // GPIO33 para validar la secuencia

// Definición de tiempos para clasificar los toques
#define TIEMPO_LARGO   800    // Tiempo mínimo en ms para considerar un toque largo
#define TIEMPO_CORTO   200    // Tiempo mínimo en ms para considerar un toque corto
#define TOLERANCIA     100    // Margen de error en la medición de toques
#define UMBRAL         361    // Sensibilidad del sensor táctil (ajustar según calibración)

static const char *TAG = "TOUCH_AUTH"; // Etiqueta para logs en consola

// Patrón esperado de autenticación: 3 toques largos, 3 cortos, 3 largos
int patronEsperado[9] = {1, 1, 1, 0, 0, 0, 1, 1, 1};
int patronUsuario[9]; // Almacena la secuencia ingresada por el usuario
int pos = 0; // Índice para almacenar los toques en patronUsuario[]
bool secuenciaCompleta = false; // Indica si la secuencia de 9 toques ha sido ingresada

void app_main(void) {
    // Inicialización del sistema de sensores táctiles
    touch_pad_init();
    touch_pad_config(PIN_SECUENCIA, UMBRAL);
    touch_pad_config(PIN_VALIDACION, UMBRAL);
    
    // Variables de estado para detectar cambios en el sensor
    bool ultimoEstadoSecuencia = false;
    int64_t inicioPulso = 0;
    bool ultimoEstadoValidacion = false;

    while (1) {
        if (!secuenciaCompleta) {
            // Leer el estado del sensor de secuencia
            bool estadoSecuencia = false;
            uint16_t valorSecuencia = 0;
            if (touch_pad_read(PIN_SECUENCIA, &valorSecuencia) == ESP_OK) {
                if (valorSecuencia < UMBRAL) {
                    estadoSecuencia = true; // El sensor está siendo tocado
                }
            }
            // Detectar el inicio del toque
            if (estadoSecuencia && !ultimoEstadoSecuencia) {
                inicioPulso = esp_timer_get_time() / 1000; // Guardar el tiempo en ms
            }
            // Detectar el final del toque
            if (!estadoSecuencia && ultimoEstadoSecuencia) {
                int duracion = (esp_timer_get_time() / 1000) - inicioPulso;
                // Clasificar el toque como largo o corto
                if (duracion >= (TIEMPO_LARGO - TOLERANCIA)) {
                    patronUsuario[pos] = 1; // Toque largo
                    ESP_LOGI(TAG, "Toque largo registrado");
                } else if (duracion >= (TIEMPO_CORTO - TOLERANCIA)) {
                    patronUsuario[pos] = 0; // Toque corto
                    ESP_LOGI(TAG, "Toque corto registrado");
                }
                pos++;
                if (pos >= 9) {
                    secuenciaCompleta = true; // Se ha ingresado la secuencia completa
                }
                vTaskDelay(pdMS_TO_TICKS(50)); // Pequeño delay para evitar falsos positivos
            }
            ultimoEstadoSecuencia = estadoSecuencia; // Actualizar estado anterior
        } else {
            // Leer el estado del sensor de validación
            bool estadoValidacion = false;
            uint16_t valorValidacion = 0;
            if (touch_pad_read(PIN_VALIDACION, &valorValidacion) == ESP_OK) {
                if (valorValidacion < UMBRAL) {
                    estadoValidacion = true; // Se está tocando el sensor de validación
                }
            }
            // Detectar si el usuario toca el sensor de validación
            if (estadoValidacion && !ultimoEstadoValidacion) {
                bool valido = true;
                // Comparar la secuencia ingresada con la esperada
                for (int i = 0; i < 9; i++) {
                    if (patronUsuario[i] != patronEsperado[i]) {
                        valido = false;
                        break;
                    }
                }
                // Mostrar resultado de la autenticación
                if (valido) {
                    ESP_LOGI(TAG, "APROBADO");
                } else {
                    ESP_LOGI(TAG, "NO APROBADO");
                }
                // Reiniciar el sistema para la próxima autenticación
                pos = 0;
                secuenciaCompleta = false;
                vTaskDelay(pdMS_TO_TICKS(500)); // Delay para evitar rebotes
            }
            ultimoEstadoValidacion = estadoValidacion; // Actualizar estado anterior
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Pequeño delay para evitar sobrecarga de CPU
    }
}
