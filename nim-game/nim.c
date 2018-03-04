#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct nodo {
    struct nodo *pai;
    int valor;
    int peso;
    struct nodo *filho1;
    struct nodo *filho2;
    struct nodo *filho3;
};

int numeroPalitos;
char jogador1;
int qtyRemovido = 0;
int numeroNodos = 0;
struct nodo *pRoot;
struct nodo *no;
int acao = 0;
int pesoAcao = 0;
/* //////////////// Arvore ////////////// */

struct nodo *criarFilhos(struct nodo *pai, int valor, int ordem){
    no = (struct nodo *)malloc(sizeof(struct nodo));
    no->peso = 0;
    no->pai = NULL;
    no->filho1 = NULL;
    no->filho2 = NULL;
    no->filho3 = NULL;
    no->pai = pai;
    no->valor = valor;
    switch(ordem){
        case 1:
            pai->filho1 = no;
        break;
        case 2:
            pai->filho2 = no;
        break;
        case 3:
            pai->filho3 = no;
        break;
    }
    return no;
}

void criarArvore(struct nodo *pai){
    numeroNodos++;
    struct nodo *no;
    if(pai->filho1 == NULL && pai->valor > 1){
        no = criarFilhos(pai, pai->valor-1, 1);
        criarArvore(no);
    }
    if(pai->filho2 == NULL && pai->valor > 2){
        no = criarFilhos(pai, pai->valor-2, 2);
        criarArvore(no);
    }
    if(pai->filho3 == NULL && pai->valor > 3){
        no = criarFilhos(pai, pai->valor-3, 3);
        criarArvore(no);
    }
}

/* //////////////// minmax /////////////// */
struct nodo *retornaFolhaEsquerda(struct nodo *no, int profundidade){
    while(no->filho1 != NULL){
        no = no->filho1; 
        profundidade*=-1;
    }
    no->peso = profundidade*=-1;
    return no;
}

struct nodo *retornaFolhaDireita(struct nodo *no){
    while((no->filho3 != NULL || no->filho2 != NULL) || no->filho1 != NULL){
        if(no->filho3 != NULL){
            no = no->filho3;
        }else if(no->filho2 != NULL){
            no = no->filho2;
        }else {
            no = no->filho1;
        }
    }
    return no;
}

void imprime (struct nodo *no) { 
    if (no == NULL) return;
    imprime(no->filho1);
    imprime(no->filho2);
    imprime(no->filho3);
    printf("valor: %d, peso: %d\n", no->valor, no->peso);
}

void setaPeso(struct nodo *no, int profundidade){
    if (no == NULL) return;
    if(no->pai != NULL) profundidade *= -1;
    if(no->filho1 == NULL) no->peso = profundidade*-1;
    setaPeso(no->filho1, profundidade);
    setaPeso(no->filho2, profundidade);
    setaPeso(no->filho3, profundidade);
    if(no->filho1 != NULL) no->peso = no->filho1->peso;
    if(no->filho2 != NULL){
        if(profundidade == 1) no->peso = no->filho1->peso > no->filho2->peso ? no->filho1->peso : no->filho2->peso;
        if(profundidade == -1) no->peso = no->filho1->peso < no->filho2->peso ? no->filho1->peso : no->filho2->peso;
    }
    if(no->filho3 != NULL){
        if(profundidade == 1) no->peso = no->peso > no->filho3->peso ? no->peso : no->filho3->peso;
        if(profundidade == -1) no->peso = no->peso < no->filho3->peso ? no->peso : no->filho3->peso;
    }
    //printf("valor: %d, peso: %d profundidade: %d\n", no->valor, no->peso, profundidade);
}

void minmax(struct nodo *no){
    if (no == NULL) return;
    if(no->peso == 1 && no->filho1 == NULL){
        struct nodo *tempNo = no;
        while(tempNo->pai->valor < numeroPalitos){
            tempNo = tempNo->pai;
        }
        if(pesoAcao == 0){
            pesoAcao = tempNo->peso;
            acao = numeroPalitos - tempNo->valor;
        }
        if(pesoAcao < 0 && tempNo->peso == 1){
            pesoAcao = tempNo->peso;
            acao = numeroPalitos - tempNo->valor;
        }
    }
    minmax(no->filho3);
    minmax(no->filho2);
    minmax(no->filho1);
}

/* //////////////// mensagens /////////// */
void mensagemInicial(){
    printf("Digite o numero de palitos: ");
    scanf("%d", &numeroPalitos);

    printf("O jogo iniciará com %d palitos\n", numeroPalitos);

    printf("Você deseja começar? Digite S para sim ou N para não:\n");

    scanf(" %c", &jogador1);
}

void mensagemPalitosRestantes(){
    if(pRoot->valor == numeroPalitos+3){
        pRoot = pRoot->filho3;
    }else if(pRoot->valor == numeroPalitos+2){
        pRoot = pRoot->filho2;
    }else{
        pRoot = pRoot->filho1;
    }
    pRoot->pai = NULL;  
    printf("resta(m) %d palito(s)\n", numeroPalitos);
}

bool calcHumanoVenceu(int jogadorVez){
    bool humanoVencedor = false;
    if(jogadorVez == 1){
        humanoVencedor = true;
    }
    return humanoVencedor;
}

void mensagenDeVitoria(int jogadorVitorioso){
    if(jogadorVitorioso == 0){
        printf("Desculpe você perdeu\n");
    }else{
        printf("Parabéns você venceu\n");
    }
}

void mensagemDeJogada(){
    if(numeroPalitos >= 3){
        printf("É sua jogada escolha remover 1,2 ou 3 palitos\n");
    }else if(numeroPalitos == 2){
        printf("É sua jogada escolha remover 1 ou 2 palitos\n");            
    }
    scanf("%d", &qtyRemovido);
    numeroPalitos -=  qtyRemovido;
    mensagemPalitosRestantes();
}

int acaoPc(){
    if((numeroPalitos%2) == 0){
        if(numeroPalitos > 3){
            return 3;
        }else {
            return 1;
        }
    }else{
        if(numeroPalitos == 3){
            return 2;
        }else{
            return 3;
        }
    }
}

void mensagemDeAcao(){
    acao = 0;
    pesoAcao = 0;
    setaPeso(pRoot, 1);
    minmax(pRoot);
    printf("Minnha vez, vou remover %d palito(s)\n", acao);
    numeroPalitos -= acao;
    mensagemPalitosRestantes();
}

bool alguemVenceu(){
    if(numeroPalitos == 1){
        return true;
    }else{
        return false;
    }
}

int main(){
    mensagemInicial();
    struct nodo root;
    root.peso = 0;
    root.valor = numeroPalitos;
    root.pai = NULL;
    root.filho1 = NULL;
    root.filho2 = NULL;
    root.filho3 = NULL;
    pRoot = &root;
    criarArvore(pRoot);
    while(numeroPalitos > 1){
        if(jogador1 == 'S'){
            mensagemDeJogada();
            if(alguemVenceu()){
                if(calcHumanoVenceu(1)){
                    mensagenDeVitoria(1);
                }else{
                    mensagenDeVitoria(0);
                }
                break;
            };
            mensagemDeAcao();
            if(alguemVenceu()){
                if(calcHumanoVenceu(0)){
                    mensagenDeVitoria(1);
                }else{
                    mensagenDeVitoria(0);
                }
            };
        }else{
            mensagemDeAcao();
            if(alguemVenceu()){
                if(calcHumanoVenceu(0)){
                    mensagenDeVitoria(1);
                }else{
                    mensagenDeVitoria(0);
                }
                break;
            };
            mensagemDeJogada();
            if(alguemVenceu()){
                if(calcHumanoVenceu(1)){
                    mensagenDeVitoria(1);
                }else{
                    mensagenDeVitoria(0);
                }
            };         
        }
    }

    return 0;
}
