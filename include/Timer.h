#include <iostream>
#include <chrono>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

class Timer {

    using Clock = std::chrono::steady_clock;
    using Marca = Clock::time_point;

    string titulo;
    Marca inicio;
    vector<pair<string, Marca>> marcas;

    float calc_delta(Marca marca) {
        return chrono::duration<double>(marca - inicio).count();
    }

public:

    Timer(string titulo) {
        this->titulo = titulo; 
        this->inicio = Clock::now();
    }

    void marcar(string label) {
        Marca marca = Clock::now();
        marcas.emplace_back(label, marca);
    }

    void imprimir_ultimo() {
     
        if (marcas.empty()) {
            cout << "Nenhuma marca registrada.\n";
            return;
        }

        auto [label, marca] = marcas.back();
            cout << label << ": " << calc_delta(marca) << "s" << endl;
    }

    void imprimir_resultado() {
        
        cout << "==" << titulo << "==" << endl;
        for (auto [label, marca] : marcas)
            cout << label << ": " << calc_delta(marca) << "s\n";
    }
};
