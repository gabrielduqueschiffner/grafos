#ifndef ARESTA_H
#define ARESTA_H

#include "types.h"

class Aresta {
public:
    Aresta();
    Aresta(NoId id_no_origem, NoId id_no_alvo, int peso=0, bool dominante=false);
    ~Aresta();

    //getters e setters
    void set_id_no_alvo(NoId id);
    void set_id_no_origem(NoId id);
    void set_peso(int peso);
    void set_domina(bool domina);

    NoId get_id_no_alvo();
    NoId get_id_no_origem();
    int get_peso();
    bool get_domina();

private:
    NoId id_no_alvo, id_no_origem;  //aresta de entrada e saida
    int peso;
    bool domina;
};

#endif //ARESTA_H
