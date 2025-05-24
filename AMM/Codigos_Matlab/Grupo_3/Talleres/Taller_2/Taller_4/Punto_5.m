clc
den = [1 9 30 40];  % s^3 + 9 s^2 + 30 s + 40
num = 50;
G= tf(num,den)

omega = 29;
s = 1j * omega;
Gjw = num / polyval(den, s);
magnitude = abs(Gjw);
phase_deg = angle(Gjw) * (180/pi);
fprintf('Magnitud en ω = %.2f rad/s: %.6f\n', omega, magnitude);
fprintf('Fase en ω = %.2f rad/s: %.2f grados\n', omega, phase_deg);

figure;bode(G);
grid on;
title('Diagrama de Bode de G(s)');

figure;step(G)
grid on;
title('Entrada STEP');
