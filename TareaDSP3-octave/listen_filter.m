

function listen_filter(filter_file, wav_file, Fs_filter = 48000)


    pkg load signal;
    % Cargar el filtro SOS desde el archivo
    Data = load(filter_file, "SOS");
    SOS = Data.SOS;  % Acceder a la matriz SOS

    % Convertir SOS a los coeficientes b y a de la función de transferencia completa
    [b, a] = sos2tf(SOS);

    % Cargar el archivo de audio .wav
    [audio, Fs_wav] = audioread(wav_file);

    % Verificar si la frecuencia de muestreo del wav difiere del filtro
    if Fs_wav ~= Fs_filter
        fprintf('Frecuencia de muestreo diferente detectada: Resampling de %d a %d Hz.\n', Fs_wav, Fs_filter);
        audio = resample(audio, Fs_filter, Fs_wav);  % Ajustar la frecuencia de muestreo
    end

    % Aplicar el filtro al archivo de audio
    filtered_audio = filter(b, a, audio);

    % Reproducir el audio original y filtrado
    fprintf('Reproduciendo audio original...\n');
    sound(audio, Fs_filter);
    pause(length(audio) / Fs_filter + 1);  % Esperar a que termine la reproducción

    fprintf('Reproduciendo audio filtrado...\n');
    sound(filtered_audio, Fs_filter);
    pause(length(filtered_audio) / Fs_filter + 1);  % Esperar a que termine la reproducción

    % Guardar el audio filtrado en un nuevo archivo .wav
    output_file = strcat('filtered_', wav_file);
    audiowrite(output_file, filtered_audio, Fs_filter);
    fprintf('Audio filtrado guardado como: %s\n', output_file);
end

