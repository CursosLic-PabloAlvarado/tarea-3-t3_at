#include <vector>
#include <iostream>

class TransferFunction {
public:
    TransferFunction(const std::vector<float>& numerator, const std::vector<float>& denominator)
        : b(numerator), a(denominator) {}

    // Aplicar el filtro a una señal de entrada
    std::vector<float> applyFilter(const std::vector<float>& input) {
        size_t N = input.size();
        size_t M = b.size();
        size_t L = a.size();
        std::vector<float> output(N, 0);

        for (size_t n = 0; n < N; ++n) {
            // Convolución del numerador
            for (size_t i = 0; i < M; ++i) {
                if (n >= i) {
                    output[n] += b[i] * input[n - i];
                }
            }
            // Convolución del denominador
            for (size_t j = 1; j < L; ++j) {  // empieza desde 1 porque a[0] = 1
                if (n >= j) {
                    output[n] -= a[j] * output[n - j];
                }
            }
        }

        return output;
    }

private:
    std::vector<float> b;  // Coeficientes del numerador
    std::vector<float> a;  // Coeficientes del denominador
};

int main() {
    // Ejemplo de coeficientes del filtro
    std::vector<float> b = {0.0675, 0.135, 0.0675};  // Numerador
    std::vector<float> a = {1.0, -1.1430, 0.4128};   // Denominador

    // Crear el filtro de transferencia
    TransferFunction tf(b, a);

    // Señal de entrada (ejemplo)
    std::vector<float> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Aplicar el filtro a la señal de entrada
    std::vector<float> output = tf.applyFilter(input);

    // Imprimir la señal filtrada
    std::cout << "Señal filtrada: ";
    for (float sample : output) {
        std::cout << sample << " ";
    }
    std::cout << std::endl;

    return 0;
}
