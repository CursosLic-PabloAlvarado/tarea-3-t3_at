function viewfreqresp(filename, Fs = 48000)

    pkg load signal;
    % Cargar la matriz SOS desde el archivo
    Data = load(filename, "SOS");
    SOS = Data.SOS;  % Acceder a la matriz SOS

    % Convertir SOS a los coeficientes b y a de la función de transferencia completa
    [b, a] = sos2tf(SOS);

    % 1. Diagrama de polos y ceros
    figure;  % Crear nueva figura
    pzmap(tf(b, a));  % Polos y ceros
    hold on;
    % Dibujar el círculo unitario
    t = linspace(0, 2 * pi, 100);
    plot(cos(t), sin(t), 'r--', 'LineWidth', 2);  % Círculo unitario con línea más gruesa
    axis equal;  % Mantener proporciones cuadradas
    xlim([-2 2]);  % Ajustar el rango en X
    ylim([-2 2]);  % Ajustar el rango en Y
    title('Diagrama de Polos y Ceros');
    grid on;

    % Evaluar la respuesta en frecuencia
    w = linspace(0, pi, 1024);  % Frecuencias en radianes
    H = polyval(b, exp(1j*w)) ./ polyval(a, exp(1j*w));  % H(z)

    % Respuesta en frecuencia: magnitud y fase
    mag = 20 * log10(abs(H));  % Magnitud en dB
    phase = -1*mod(angle(H) * (180 / pi), 360);  % Fase en grados, ajustar a [0, 360]

    % 2. Gráfico de magnitud (eje logarítmico en frecuencias)
    figure;  % Crear nueva figura para la magnitud
    semilogx(w * (Fs / (2 * pi)), mag, 'LineWidth', 2);  % Convertir w a Hz, usar semilogx para escala logarítmica
    xlim([50 5000]);  % Limitar el rango de frecuencias de 10^0 a 10^4
    ylim([-100 0]);
    title('Respuesta en Frecuencia (Magnitud)');
    xlabel('Frecuencia (Hz)');
    ylabel('Magnitud (dB)');
    grid on;

    % 3. Gráfico de fase (eje logarítmico en frecuencias)
    figure;  % Crear nueva figura para la fase
    semilogx(w * (Fs / (2 * pi)), phase, 'LineWidth', 2);  % Convertir w a Hz, usar semilogx para escala logarítmica
    xlim([50 5000]);  % Limitar el rango de frecuencias de 10^0 a 10^4
    ylim([-360 0]);  % Limitar el rango de fase a [0, 360] grados
    title('Respuesta en Frecuencia (Fase)');
    xlabel('Frecuencia (Hz)');
    ylabel('Fase (grados)');
    grid on;
end

