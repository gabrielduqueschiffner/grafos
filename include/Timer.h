#include <iostream>
#include <chrono>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Timer {

    using Clock = std::chrono::steady_clock;
    using Marca = Clock::time_point;

    string titulo;
    Marca inicio;
    vector<pair<string, Marca>> marcas;

    double calc_delta(Marca marca2, Marca marca1) {
        return chrono::duration<double>(marca2 - marca1).count();
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
            cout << label << ": " << calc_delta(marca, inicio) << "s" << endl;
    }

    void imprimir_resultado() {

        if (marcas.size() < 2)
            throw runtime_error("Sem marcas suficiente");
        
        for (int i=1; i<marcas.size(); i++) {

            cout << marcas[i].first << ", " << calc_delta(marcas[i].second, marcas[i-1].second) << "s\n";
        }
    }
};
