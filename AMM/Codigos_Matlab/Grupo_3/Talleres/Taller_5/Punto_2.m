EJERCICICIO 2

% Función de transferencia del horno
G = tf([1], [1 10 20]);
% Controlador PID inicial
Kp = 110;
Ki = 200;
Kd = 10;
PID = pid(Kp, Ki, Kd);
% Sistema en lazo cerrado
T = feedback(PID * G, 1);
t = 20;

% Simulación de la respuesta al escalón
subplot(2, 1, 1);  % 2 filas, 1 columna, gráfica 1
step(G);
title('Ejercicio 2: Respuesta al escalón del sistema sin controlador');
grid on;

subplot(2, 1, 2);  % 2 filas, 1 columna, gráfica 2
step(T, 3);
grid on;
title('Ejercicio 2: Respuesta al escalón del sistema con PID');
xlabel('Tiempo (s)');
ylabel('Amplitud');
