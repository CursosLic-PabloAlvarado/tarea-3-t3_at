% Definir coeficientes
coefs = {
    [0.88489099304085195, -1.7647259369167299, 0.88489099279944872, 1, -1.9447696737414277, 0.96118976527688038],
    [1, -1.9962282681026606, 1.0000000006886136, 1, -1.9902746344470237, 0.99111829673050256],
    [1, -1.9975106328926273, 0.99999999958419417, 1, -1.8137023593689499, 0.81712922359906082]
};

% Tamaño de la señal
size = 256;

% Crear el pulso unitario
input_signal = zeros(1, size);
input_signal(1) = 1;  % Primer valor es 1, los demás 0

% Inicializar la salida
output_signal = input_signal;  % Comenzamos con el pulso unitario

% Aplicar el filtro en cascada
for i = 1:length(coefs)
    b = coefs{i}(1:3);  % Coeficientes de numerador
    a = coefs{i}(4:6);  % Coeficientes de denominador

    % Filtrar la señal
    output_signal = filter(b, a, output_signal);
end

% Imprimir la salida
disp('Output:');
disp(output_signal);

