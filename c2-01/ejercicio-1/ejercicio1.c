#include <stdio.h>             
#include <string.h>            // Para manejo de cadenas
#include <stdlib.h>            // Para atoi
#include <ctype.h>             // Para isdigit
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_NUM UART_NUM_0 // Puerto UART que usaremos
#define TAMANO_BUF 1024     // Tamaño del Buffer

// Función que verifica si la entrada es un número entero desde el 00 al 99
int numero_valido(const char* str){
    if (str == NULL || *str =='\0' || *str == '-') return 0;
    // Verificamos que todos los carateres sean dígitos numéricos
    for (int i = 0; str[i]; i++){
        if(!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

void app_main(){
    // Inicializar el puerto UART
    uart_driver_install(UART_NUM, TAMANO_BUF, 0, 0, NULL, 0);
    uint8_t datos[TAMANO_BUF];
    
    // Mostrar mensaje de bienvenida al inicio
    const char* bienvenida = "Ingresar numeros entre el 00 y 99 separados por coma\r\n";
    uart_write_bytes(UART_NUM, bienvenida, strlen(bienvenida));

    while (1){
        // Lee desde UART y guarda los datos en datos
        int len = uart_read_bytes(UART_NUM, datos, TAMANO_BUF, pdMS_TO_TICKS(10000)); // Tiempo de espera entre datos

        if(len > 0){
            datos[len] = '\0'; // Final de la cadena
            
            // Eliminamos salto de linea si esta se llega a presentar
            char *nl = strchr((char *)datos, '\n');
            if(nl) *nl = '\0';
            char *linea = (char *)datos;

            // Si la linea esta vacia, repetir el intento
            if(strlen(linea) == 0) {
                printf("Linea vacia. Intente de nuevo\n");
                continue;
            }   

            // Variables a analisis
            int ultimo = 0; // Se actualiza cada que ingresa un nuevo numero
            int menor = 100; // El limite es 99 por lo cual el numero menor de los que ingrese ocupara este espacio
            int mayor = -1; // El numero mayor a -1 lo remplazara y asi cada vez que ingrese un numero nuevo
            int suma = 0;
            int cantidad = 0;

            // Variables para almacenar los datos que se deben ignorar
            char ignorados[256] = "";       // Acumula los valores ignorados como texto
            int hay_ignorados = 0;          // Bandera para saber si hay algo que mostrar

            // Dividimos la cadena por comas para obtener cada numero
            char *token = strtok(linea, ",");

            while (token != NULL){
                while (*token == ' ') token++; // Eliminamos espacios en blanco 

                // Verificamos si el token actual es un numero valido 
                if (numero_valido(token)){
                    int numero = atoi(token); // convierte la cadena en numero entero 
                    //verificamos que este dentro del rango 
                    if (numero >= 0 && numero <= 99){
                        ultimo = numero;                    //Guardamos el ultimo numero valido
                        if (numero < menor) menor = numero; // Actualizamos el menor 
                        if (numero > mayor) mayor = numero; // Actualizamos el mayor 
                        suma += numero;                     // Suma para el promedio
                        cantidad++;                         // cuenta cuantos validos van
                    } else {
                        // Si está fuera de rango, lo marcamos como ignorado
                        if (hay_ignorados) strcat(ignorados, ", ");
                        strcat(ignorados, token);
                        hay_ignorados = 1;
                    }
                } else {
                    // si no es valido
                    if (hay_ignorados) strcat(ignorados, ", ");
                    strcat(ignorados, token);
                    hay_ignorados = 1;
                }

                token = strtok(NULL, ","); // Pasar al siguiente numero
            }

            if (cantidad > 0){
                float promedio = (float)suma / cantidad;
                printf("\n---RESULTADO----\n");
                printf("Ultimo:%d\n", ultimo);
                printf("Minimo:%d\n", menor);
                printf("Maximo:%d\n", mayor);
                printf("Promedio:%.2f\n", promedio);
            } else {
                printf("No se ingresaron numero validos.\n\n");
            }

            // Mostrar datos ignorados solo si hubo al menos uno
            if (hay_ignorados) {
                printf("Ignorados: %s\n", ignorados);
            }
        }

        // Delay antes de otra entrada 
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}