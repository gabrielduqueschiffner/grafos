#pragma once
#include <iostream>
#include <random>

using namespace std;

class Aleatorio {

private:

    mt19937 gen;
    int seed;

public:

    Aleatorio(int seed) {
        this->seed = seed;
        gen = mt19937(seed);
    }

    Aleatorio() : Aleatorio(random_device{}()) {}

    int gerar_inteiro(int max) {

        /* Gera um inteiro aleatório entre 0 e max. Max incluso. */

        uniform_int_distribution<> dist(0, max);
        return dist(gen);
    }

    float gerar_float(float max) {
        uniform_real_distribution<> dist(0, max);
        return dist(gen);
    }

    int get_seed() {
        return seed;
    }
};
