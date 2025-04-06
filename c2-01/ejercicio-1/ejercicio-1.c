/* Brian Mendez-Eduardo Meriño-Mateo Peña
En un proyecto de telemetría se tiene un caudalímetro que envía por puerto
 serial cada periodo de tiempo la cantidad de caudal detecta, el cual es un
 número entero no mayor de 2 dígitos (es decir, se enviará un número entre
 00 y 99)
 Se recibirá el número por puerto serial, se debe imprimir por serial la
 siguiente información:
 1. El número mínimo recibido.
 2. El número mayor recibido.
 3. El último número recibido.
 4. El promedio de todos los números recibidos.
 Un ejemplo de este programa es el siguiente:
 Último: 10. Minimo: 10. Máximo: 10. Promedio: 10.00
 Último: 1. Minimo: 1. Máximo: 10. Promedio: 5.50
 Último: 27. Minimo: 1. Máximo: 27. Promedio: 12.67
 En el anterior ejemplo, se han enviado por serial 3 números en orden que
 son 10, 1 y 27. Donde al recibir el tercer dato (27), el número menor en ese
 momento es 1, el número mayor es 10, el último número recibido es 27 y el
 promedio de la sumatoria de todos los números recibidos es (10 + 1 + 27)/3
 = 12.667
 Si se recibe algo diferente entre 00 y 99 (sea número o letra) se debe
 ignorar.*/
#include <stdio.h>             
#include <string.h>            // Para manejo de cadenas
#include <stdlib.h>            // Para atoi
#include <ctype.h>             // Para isdigit
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_NUM UART_NUM_0 // Puerto UART que usaremos
#define TAMANO_BUF 1024     // Tamaño del Buffer

// Función que veridica si la entrada es un número entero desde el 00 al 99
int numero_valido(const char* str){

    if (str == NULL || *str =='\0' || *str == '-') return 0;
    // Verificamos que todos los carateres sean dígitos numéricos
    for (int i = 0;str[i];i++){
        if(!isdigit((unsigned char)str[i])) return 0;
    }
     return 1;
}

void app_main(){
    // Inicializar el puerto UART
    uart_driver_install(UART_NUM, TAMANO_BUF, 0,0, NULL,0);
    uint8_t datos[TAMANO_BUF];
    
     // Mostrar mensaje de bienvenida al inicio
    const char* bienvenida = "Ingersar numeros entre el 00 y 99 separados por coma\r\n";
    uart_write_bytes(UART_NUM, bienvenida, strlen(bienvenida));

    while (1){
         // Lee desde UART y guarda os datis en datos
          int len = uart_read_bytes(UART_NUM, datos,TAMANO_BUF,pdMS_TO_TICKS(10000));// Tiempo de espera entre datos

          if(len>0){
            datos[len]= '\0';// Final de la cadena
            
            // Eliminamos salto de linea si esta se llega a presentar
            char *nl =strchr((char *)datos, '\n');
            if(nl) *nl='\0';
            char *liena =(char *)datos;

            // Si la linea esta vacia, repetir el intento
            if(strlen(liena) == 0) {
                 printf("Linea vacia. Intente de nuevo\n");
                 continue;
            }   

            // Variables a analisis
            int ultimo = 0; // Se actualiza cada que ingresa un nuevo numero
            int menor = 100;// El limite es 99 por lo cual el numero menor de los que ingrese ocupara esyte esapacio
            int mayor = -1; // El numero mayor a -1 lo remplazara y asi cada vez que ingrese un numero nuevo
            int suma = 0;
            int cantidad = 0;     

            // Dividimos la cadena por comas para obtener cada numero
            char *token = strtok(liena, ",");

            while (token != NULL){
                while (*token == ' ') token++; // Eliminamos espacios en blanco 
                // Verificamos si el token actual es un numero valido 
                if (numero_valido(token)){
                    int numero = atoi(token);// convierte la cadena en numero entero 
                    //verificamos que este dentro del rango 
                    if (numero >= 0 && numero <= 99 ){
                        ultimo = numero;                    //Guardamos el ultimo numero valido
                        if (numero < menor) menor = numero; // Acrualizamos el menor 
                        if (numero > mayor) mayor = numero; // Actualizamos el mayo 
                        suma += numero;                     // Suma para el promedio
                        cantidad++;                         // cuenta cuantos validos van
                    }else {
                        printf("Numero fuera de rango (0-99): %d. Ignorado.\n", numero);
                    }
                } else{
                 // si no es valido
                 printf("Entrada invalida%s. Ignorado.\n",token);   
                }

                token = strtok(NULL, ",");// Pasar al iguinte numero
            }
            if ( cantidad > 0){
                float Promedio = (float)suma / cantidad;
                printf("\n---RESULTADO----\n");
                printf("Ultimo:%d\n",ultimo);
                printf("Minimo:%d\n",menor);
                printf("Maximo:%d\n",mayor);
                printf("Promedio:%.2f\n",Promedio);
            }else{
                printf("No se ingresaron numero validos.\n\n");
            }
          }
          // Daley antes de otra entrada 
          vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}
