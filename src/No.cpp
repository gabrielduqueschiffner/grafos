#include "No.h"
#include "types.h"

No::No() {
    id = '\0'; // Inicializa o id como nulo
    peso = 0; // Inicializa o peso como 0
    arestas = vector<Aresta*>(); // Inicializa a lista de arestas vazia
}

No::No(NoId id_no, int peso) {
    this->id = id_no; 
    this->peso = peso; 
    this->arestas = vector<Aresta*>(); 
}

No::~No() {
    for (Aresta* aresta : arestas) {
        delete aresta; // Libera a memória alocada para as arestas
    }
}

void No::set_peso(int peso) {
    this->peso = peso;
}

void No::set_id(NoId id) {
    this->id = id;
}
NoId No::get_id() {
    return id;
}
int No::get_peso() {
    return peso;
}
vector<Aresta*> No::get_arestas() {
    return arestas;
}

void No::adiciona_aresta(NoId id_no_origem, NoId id_no_alvo, int peso) {

    Aresta* aresta = new Aresta();
    aresta->set_id_no_origem(id_no_origem);
    aresta->set_id_no_alvo(id_no_alvo);
    aresta->set_peso(peso);

    arestas.push_back(aresta);
}

void No::adiciona_aresta(Aresta* aresta) {
    arestas.push_back(aresta);
}