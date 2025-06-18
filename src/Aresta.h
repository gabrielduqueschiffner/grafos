#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta {
public:
    Aresta();
    ~Aresta();

    //getters e setters
    void set_id_no_alvo(char id);
    void set_id_no_origem(char id);
    void set_peso(int peso);

    char get_id_no_alvo();
    char get_id_no_origem();
    int get_peso();

    char id_no_alvo, id_no_origem;  //aresta de entrada e saida
    int peso;
    
};



#endif //ARESTA_H
