#include "Aresta.h"


void Aresta::set_id_no_alvo(char id) {
    id_no_alvo = id;
}
void Aresta::set_id_no_origem(char id) {
    id_no_origem = id;
}
void Aresta::set_peso(int peso) {
    this->peso = peso;
}
char Aresta::get_id_no_alvo() {
    return id_no_alvo;
}
char Aresta::get_id_no_origem() {
    return id_no_origem;
}
int Aresta::get_peso() {
    return peso;
}
