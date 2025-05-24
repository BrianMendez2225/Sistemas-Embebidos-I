EJERCICIO 1

% Función de transferencia del horno
G = tf([1 2], [1 0 4]);
% Controlador PID inicial
Kp = 3000;
Ki = 220;
Kd = 50;
PID = pid(Kp, Ki, Kd);
% Sistema en lazo cerrado
T = feedback(PID * G, 1);
t = 3;

% Simulación de la respuesta al escalón
subplot(2, 1, 1);  % 2 filas, 1 columna, gráfica 1
step(G, 10);
title('Ejercicio 1: Respuesta al escalón del sistema sin controlador');
grid on;

subplot(2, 1, 2);  % 2 filas, 1 columna, gráfica 2
step(T, 3);
grid on;
title('Ejercicio 1: Respuesta al escalón del sistema con PID');
xlabel('Tiempo (s)');
ylabel('Amplitud');
