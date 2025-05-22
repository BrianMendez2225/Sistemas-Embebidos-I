syms K EPS
% Para hallar la tabla de Routh-Hurwitz aquí se debe colocar los coeficientes del polinomio característico
% de manera descendiente en la función routh, resolvamos los ejemplos usando el software para verificar:

%% Sistema 1:
disp('Sistema 1:');
estabilidad = routh([1 4 6 8 5 2], EPS);
disp(estabilidad);

%% Sistema 2:
disp('Sistema 2:');
estabilidad2 = routh([1 2 3 4 5], EPS);
disp(estabilidad2);