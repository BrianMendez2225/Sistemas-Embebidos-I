/* Brian Mendez - Eduardo Meriño - Mateo Peña 
1. Se puede calcular el cuadrado de un número N al sumar los N primeros
 números impares, así 7 al cuadrado es igual a 1+3+5+7+9+11+13=49,
 donde la serie del 1 al 13 son los primeros 7 números impares.
 2. Describir una solución para el microcontrolador que calcule el cuadrado
 de un número recibido por puerto serial e imprima el resultado por
 serial.
 3. Si se recibe algo diferente a un número entero se debe ignorar.
*/
#include <stdio.h>  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "string.h"

#define UART_NUM UART_NUM_0  // Puerto UART que usaremos
#define TAMANO_BUF 1024      // Tamaño del buffer para recepción de datos, puede reducirse para ahorrar memoria

// Función que calcula el cuadrado de un número sumando los primeros N números impares
int calcular_cuadrado_por_suma(int n) {
    int suma = 0, impar = 1;
    for (int i = 0; i < n; i++) {  // Iterar N veces
        suma += impar;  // Sumar el número impar actual
        impar += 2;  // Pasar al siguiente número impar
    }
    return suma;
}

// Función que verifica si la entrada es un número entero positivo
int es_numero_entero_positivo(const char* str) {
    // Verificar que todos los caracteres sean dígitos numéricos
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[0] == '-' || str[i] < '0' || str[i] > '9') {
            return 0;  // No es un número entero positivo
        }
    }
    return 1;  // Es un número entero positivo
}

void app_main() {
    // Inicializar el puerto UART
    uart_driver_install(UART_NUM, TAMANO_BUF, 0, 0, NULL, 0);
    uint8_t datos[TAMANO_BUF]; // Buffer para almacenar los datos recibidos

    // Mostrar mensaje de bienvenida al inicio
    const char* bienvenida = "Calculadora de cuadrados\r\n";
    uart_write_bytes(UART_NUM, bienvenida, strlen(bienvenida));

    while (1) {
        // Leer datos recibidos por UART
        int longitud = uart_read_bytes(UART_NUM, datos, TAMANO_BUF, pdMS_TO_TICKS(750));
        if (longitud > 0) {
            datos[longitud] = '\0';  // Convertir los datos a cadena terminada en null

            // Verificar si los datos recibidos son un número entero positivo
            if (es_numero_entero_positivo((char *)datos)) {
                int n = atoi((char *)datos);  // Convertir la cadena a número entero

                char mensaje[50];
                    sprintf(mensaje, "Resultado: %d\r\n", calcular_cuadrado_por_suma(n));  // Formatear el mensaje de salida
                    uart_write_bytes(UART_NUM, mensaje, strlen(mensaje));  // Enviar el resultado por UART
            }
            // Si no es un número entero positivo, no hacer nada (se ignora)
        }
        vTaskDelay(pdMS_TO_TICKS(100));  // Pequeño retardo para evitar sobrecarga
    }
}
