#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"
#include "types.h"


using namespace std;
class No {
public:
    No();
    ~No();

    void set_peso(int peso);
    void set_id(NoId id);

    NoId get_id();
    int get_peso();
    vector<Aresta*> get_arestas();

    void adiciona_aresta(NoId id_no_origem, NoId id_no_alvo, int peso = 0);

    NoId id;
    int peso;
    vector<Aresta*> arestas;
};



#endif //NO_H
