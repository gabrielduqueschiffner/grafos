#include "No.h"


void No::set_peso(int peso) {
    this->peso = peso;
}

void No::set_id(char id) {
    this->id = id;
}
char No::get_id() {
    return id;
}
int No::get_peso() {
    return peso;
}
vector<Aresta*> No::get_arestas() {
    return arestas;
}

void No::adiciona_aresta(Aresta* aresta) {
    arestas.push_back(aresta);
}