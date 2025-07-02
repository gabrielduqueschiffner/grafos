#include "Aresta.h"
#include "types.h"

Aresta::Aresta() {
    id_no_alvo = '\0'; // Inicializa o id do nó alvo como nulo
    id_no_origem = '\0'; // Inicializa o id do nó origem como nulo
    peso = 0; // Inicializa o peso como 0
}
Aresta::~Aresta() {
    // Não há necessidade de liberar memória aqui, pois Aresta não aloca memória dinâmica
    // A memória será liberada quando o objeto Aresta for destruído
}

void Aresta::set_id_no_alvo(NoId id) {
    id_no_alvo = id;
}
void Aresta::set_id_no_origem(NoId id) {
    id_no_origem = id;
}
void Aresta::set_peso(int peso) {
    this->peso = peso;
}
NoId Aresta::get_id_no_alvo() {
    return id_no_alvo;
}
NoId Aresta::get_id_no_origem() {
    return id_no_origem;
}
int Aresta::get_peso() {
    return peso;
}
