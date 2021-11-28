#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "btree.h"
#include "btree.cpp"

using std::string;

#define TRUE 1
#define FALSE 0

const int max_qntd_nos = 8;

struct rede_no {
    int endereco; //Endereço identificador do nó
    bTree banco; //Árvore que representa o banco de dados
};

struct rede{
    int nos_enderecos[8]={0}; //Vetor que indica se o nó com endereço do índice já está ocupado
    rede_no nos[8]; //Vetor com os próprios nós
    int matriz_conexoes [8][8]; //Matriz que determina se existe conexão entre nós, coloquei 8x8 porque é pot de 2
};

rede add_no(int endereco, rede rede){
    if(endereco > 7 || rede.nos_enderecos[endereco] == 1){
        printf("Endereço inválido ou não disponível!!!\n");
    }
    else{
        rede.nos_enderecos[endereco] = 1;
        rede.nos[endereco].endereco = endereco;
        rede.nos[endereco].banco = btCriar(4);
    }
    return rede;
}

rede add_chave(int endereco, int chave, rede rede){
    if(endereco > 7 || rede.nos_enderecos[endereco] == 0){
        printf("Endereço inválido ou não inicializado!!!\n");
    }
    else{
        btInserir(rede.nos[endereco].banco, chave);
    }
    return rede;
}

void busca_chave(int endereco, int chave, rede rede){
    if(endereco > 7 || rede.nos_enderecos[endereco] == 0){
        printf("Endereço inválido ou não inicializado!!!\n");
    }
    else{
        int resultado = 0;
        resultado = btBuscar(rede.nos[endereco].banco, chave);
        if(resultado == 1){
            printf("A busca pela chave %d no banco %d foi bem sucedida\n", endereco, chave);
        }
        else{
            printf("A busca pela chave %d no banco %d não foi bem sucedida\n", endereco, chave);
        }
    }
}

void verifica_estrutura(int endereco, rede rede){
    if(endereco > 7 || rede.nos_enderecos[endereco] == 0){
        printf("Endereço inválido ou não inicializado!!!\n");
    }
    else{
        showtree (rede.nos[endereco].banco);
    }
}

int main(int argc, char** argv){
    //Teste 1 - Criando rede e adicionando nó em endereço não permitido
    rede rede;
    rede = add_no(10, rede);
    //Teste 2 - Adicionando nó com uma chave à rede e buscando tal chave
    rede = add_no(2, rede);
    add_chave(2, 0, rede);
    busca_chave(2, 0, rede);
    //Teste 3 - Após mais algumas adições verificar se a estrutura está de árvore B
    add_chave(2, 1, rede);
    add_chave(2, 2, rede);
    add_chave(2, 3, rede);
    add_chave(2, 4, rede);
    add_chave(2, 5, rede);
    add_chave(2, 6, rede);
    add_chave(2, 7, rede);
    verifica_estrutura(2, rede);
    return 0;
}