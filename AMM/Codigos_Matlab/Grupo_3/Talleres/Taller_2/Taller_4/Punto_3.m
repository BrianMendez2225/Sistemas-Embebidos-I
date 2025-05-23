clc
den = [4 2 1 5 10];
num = 5 10;
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
