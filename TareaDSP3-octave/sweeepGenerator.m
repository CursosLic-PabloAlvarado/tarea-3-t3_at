

Fs = 48000;   % Frecuencia de muestreo (samples/segundo)
T = 5;        % Duración en segundos
Fmin = 50;    % Frecuencia mínima del barrido en Hz
Fmax = 5000;  % Frecuencia máxima del barrido en Hz
t = linspace(0, T, T * Fs);  % Vector de tiempo

% Generar el barrido de frecuencia
sweep_signal = sin(2 * pi * Fmin * ((Fmax/Fmin).^(t/T) - 1) / log(Fmax/Fmin));

% Guardar el audio en formato WAV
audiowrite('sweep.wav', sweep_signal, Fs);

% Reproducir el barrido
sound(sweep_signal, Fs);

