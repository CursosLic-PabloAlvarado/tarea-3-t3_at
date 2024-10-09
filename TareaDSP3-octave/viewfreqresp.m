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
    plot(cos(t), sin(t), 'r--', 'LineWidth', 1);  % Círculo unitario con línea más gruesa
    axis equal;  % Mantener proporciones cuadradas
    xlim([-2 2]);  % Ajustar el rango en X
    ylim([-2 2]);  % Ajustar el rango en Y
    title('Diagrama de Polos y Ceros');
    grid on;

   % Generar frecuencias en una escala logarítmica para cubrir el rango de 10^0 a 10^4 Hz
    w = logspace(log10(1), log10(24000), 1024) * (2 * pi / Fs);  % Frecuencias en radianes por muestra

    % Calcular la respuesta en frecuencia
    H = polyval(b, exp(1j*w)) ./ polyval(a, exp(1j*w));  % H(z)

    % Convertir w a Hz
    freqs_in_hz = w * (Fs / (2 * pi));

    % 2. Gráfico de magnitud (eje logarítmico en frecuencias)
    % Crear nueva figura para la magnitud
    figure;
    set(gcf, 'Position', [100, 100, 800, 300]);  % Ajustar la posición y tamaño de la ventana (más horizontal)
    semilogx(freqs_in_hz, 20 * log10(abs(H)), 'LineWidth', 1.5);  % Usar semilogx para escala logarítmica
    xlim([1 24000]);  % Limitar el rango de frecuencias de 10^0 a 10^4
    ylim([-100 0]);
    title('Respuesta en Frecuencia (Magnitud)');
    xlabel('Frecuencia (Hz)');
    ylabel('Magnitud (dB)');
    grid on;

    % Crear nueva figura para la fase
    figure;
    set(gcf, 'Position', [100, 100, 800, 300]);  % Ajustar la posición y tamaño de la ventana (más horizontal)
    semilogx(freqs_in_hz, mod(angle(H) * (180 / pi), -360), 'LineWidth', 1.5);  % Convertir w a Hz, usar semilogx para escala logarítmica
    xlim([1 24000]);  % Limitar el rango de frecuencias de 10^0 a 10^4
    ylim([-360 0]);  % Limitar el rango de fase a [0, 360] grados
    title('Respuesta en Frecuencia (Fase)');
    xlabel('Frecuencia (Hz)');
    ylabel('Fase (grados)');
    grid on;


end
