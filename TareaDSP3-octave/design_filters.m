% design_filters.m
% Script para diseñar filtros de tercer orden con los métodos:
% ellip, butter, cheby1 y cheby2.

pkg load signal;

Fs = 48000;  % Frecuencia de muestreo en Hz
N = 3;       % Orden del filtro
Rp = 1;      % Rizado en banda de paso en dB
Rs = 50;     % Atenuación en banda de rechazo en dB

% Frecuencias de corte
fc_low = 440;  % Frecuencia de corte para filtro paso bajo
fc_high = 600; % Frecuencia de corte para filtro paso alto
fc1_band = 220; % Frecuencia de corte inferior para paso banda y supresor
fc2_band = 1000; % Frecuencia de corte superior para paso banda y supresor

% Normalización de frecuencias de corte
Wn_low = fc_low / (Fs / 2);    % Normalización de frecuencia de corte paso bajo
Wn_high = fc_high / (Fs / 2);  % Normalización de frecuencia de corte paso alto
Wn_band = [fc1_band,fc2_band]/ (Fs / 2);  % Normalización para paso banda y supresor
Wn_band_low = fc1_band / (Fs / 2);  % Normalización para paso banda y supresor bajo
Wn_band_high = fc2_band / (Fs / 2);  % Normalización para paso banda y supresor alto

% 1. Filtros paso bajo
fprintf('Filtros Paso Bajo (fc = 440 Hz)\n');

[b, a] = ellip(N, Rp, Rs, Wn_low, 'low');
SOS = tf2sos(b, a);
save("ellip_lowpass.mat", "SOS")


[b, a] = butter(N, Wn_low, 'low');
SOS = tf2sos(b, a);
save("butter_lowpass.mat", "SOS")


[b, a] = cheby1(N, Rp, Wn_low, 'low');
SOS = tf2sos(b, a);
save("cheby1_lowpass.mat", "SOS")

[b, a] = cheby2(N, Rs, Wn_low, 'low');
SOS = tf2sos(b, a);
save("cheby2_lowpass.mat", "SOS")

% 2. Filtros paso alto
fprintf('Filtros Paso Alto (fc = 600 Hz)\n');
[b, a] = ellip(N, Rp, Rs, Wn_high, 'high');
SOS = tf2sos(b, a);
save("ellip_highpass.mat", "SOS")

[b, a] = butter(N, Wn_high, 'high');
SOS = tf2sos(b, a);
save("butter_highpass.mat", "SOS")

[b, a] = cheby1(N, Rp, Wn_high, 'high');
SOS = tf2sos(b, a);
save("cheby1_highpass.mat", "SOS")

[b, a] = cheby2(N, Rs, Wn_high, 'high');
SOS = tf2sos(b, a);
save("cheby2_highpass.mat", "SOS")

% 3. Filtros paso banda
fprintf('Filtros Paso Banda (fc1 = 220 Hz, fc2 = 1000 Hz)\n');
[b, a] = ellip(N, Rp, Rs, [Wn_band_low, Wn_band_high]);
SOS = tf2sos(b, a);
save("ellip_bandpass.mat", "SOS")

[b, a] = butter(N, Wn_band, 'bandpass');
SOS = tf2sos(b, a);
save("butter_bandpass.mat", "SOS")

[b, a] = cheby1(N, Rp, [Wn_band_low, Wn_band_high]);
SOS = tf2sos(b, a);
save("cheby1_bandpass.mat", "SOS")

[b, a] = cheby2(N, Rs, [Wn_band_low, Wn_band_high]);
SOS = tf2sos(b, a);
save("cheby2_bandpass.mat", "SOS")

% 4. Filtros supresores de banda
fprintf('Filtros Supresores de Banda (fc1 = 220 Hz, fc2 = 1000 Hz)\n');
[b, a] = ellip(N, Rp, Rs, Wn_band, 'stop');
SOS = tf2sos(b, a);
save("ellip_stoppass.mat", "SOS")

[b, a] = butter(N, Wn_band, 'stop');
SOS = tf2sos(b, a);
save("butter_stoppass.mat", "SOS")

[b, a] = cheby1(N, Rp, Wn_band, 'stop');
SOS = tf2sos(b, a);
save("cheby1_stoppass.mat", "SOS")

[b, a] = cheby2(N, Rs, Wn_band, 'stop');
SOS = tf2sos(b, a);
save("cheby2_stoppass.mat", "SOS")

