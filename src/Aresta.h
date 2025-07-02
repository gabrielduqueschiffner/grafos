#ifndef ARESTA_H
#define ARESTA_H

#include "types.h"

using namespace std;
class Aresta {
public:
    Aresta();
    ~Aresta();

    //getters e setters
    void set_id_no_alvo(NoId id);
    void set_id_no_origem(NoId id);
    void set_peso(int peso);

    NoId get_id_no_alvo();
    NoId get_id_no_origem();
    int get_peso();

    NoId id_no_alvo, id_no_origem;  //aresta de entrada e saida
    int peso;
    
};



#endif //ARESTA_H
