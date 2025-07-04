#include "Gerenciador.h"
#include "types.h"

void Gerenciador::comandos(Grafo* grafo) {

    cout <<

        "\n => Digite uma das opcoes abaixo e pressione enter:"
        "\n (a) Fecho transitivo direto de um no"
        "\n (b) Fecho transitivo indireto de um no"
        "\n (c) Caminho minimo (Djikstra)"
        "\n (d) Caminho minimo (Floyd)"
        "\n (e) Arvore Geradora Minima (Algoritmo de Prim)"
        "\n (f) Arvore Geradora Minima (Algoritmo de Kruskal)"
        "\n (g) Arvore de caminhamento em profundidade"
        "\n (h) Raio, diametro, centro e periferia do grafo"
        "\n (0) Sair;"

    << endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            NoId id_no = get_id_entrada();
            vector<NoId> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            //  grafo->imprime_vector(fecho_transitivo_direto);
            cout<<endl<<endl;
           

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                grafo->exportar_vector_para_arquivo(fecho_transitivo_direto, "fecho_trans_dir.txt");
                cout<<endl<<endl;
            }

            break;
        }

        case 'b':{

            NoId id_no = get_id_entrada();
            vector<NoId> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }

        case 'c': {

            NoId id_no_1 = get_id_entrada();
            NoId id_no_2 = get_id_entrada();
            vector<NoId> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            grafo->imprime_vector(caminho_minimo_dijkstra);
            cout<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                grafo->exportar_vector_para_arquivo(caminho_minimo_dijkstra, "caminho_minimo_dijkstra.txt");
                cout<<endl;
            }


            break;
        }

        case 'd': {

            NoId id_no_1 = get_id_entrada();
            NoId id_no_2 = get_id_entrada();
            vector<NoId> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }
        case 'e': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->get_ordem()) {

                vector<NoId> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
                }

                delete arvore_geradora_minima_prim;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->get_ordem()) {

                vector<NoId> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                arvore_geradora_minima_kruskal->imprime_grafo();
                cout<<endl<<endl;

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                   arvore_geradora_minima_kruskal->exportar_grafo_para_arquivo(arvore_geradora_minima_kruskal, "agm_kruskal.txt");
                   
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            NoId id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            arvore_caminhamento_profundidade->imprime_grafo();
            cout<<endl<<endl;
            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
              grafo->exportar_grafo_para_arquivo(arvore_caminhamento_profundidade, "arvore_caminhamento_profundidade.txt");
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            vector<NoId> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }
        case 'i': {

            vector<NoId> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }

        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

NoId Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    NoId id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<NoId> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<NoId> ids = {};
    while((int)ids.size() < tam) {
        NoId id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->get_lista_adj()) {
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if (repetido) {
                    cout<<"Valor repetido"<<endl;
            } else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}