#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No();
    ~No();

    void set_peso(int peso);
    void set_id(char id);

    char get_id();
    int get_peso();
    vector<Aresta*> get_arestas();

    void adiciona_aresta(char id_no_origem, char id_no_alvo, int peso = 0);

    char id;
    int peso;
    vector<Aresta*> arestas;
};



#endif //NO_H
