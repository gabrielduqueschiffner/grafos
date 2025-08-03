
#include "Aleatorio.h"
#include "Grafo.h"

class Guloso {

public:

    Guloso();
    Guloso(float alfa);
    Guloso(float alfa, int seed);
    Grafo* conjunto_dominante_arestas(Grafo* grafo);

private:

    float alfa;
    Aleatorio* aleatorio;
};