EJERCICIO 4

% Función de transferencia del horno
G = tf([542 86], [33 5 21]);
% Controlador PID inicial
Kp = 100;
Ki = 0.24;
Kd = 0.12;
PID = pid(Kp, Ki, Kd);
% Sistema en lazo cerrado
T = feedback(PID * G, 1);
t = 0.5;

% Simulación de la respuesta al escalón
subplot(2, 1, 1);  % 2 filas, 1 columna, gráfica 1
step(G, 10);
title('Ejercicio 4: Respuesta al escalón del sistema sin controlador');
grid on;

subplot(2, 1, 2);  % 2 filas, 1 columna, gráfica 2
step(T, 0.25);
grid on;
title('Ejercicio 4: Respuesta al escalón del sistema con PID');
xlabel('Tiempo (s)');
ylabel('Amplitud');
