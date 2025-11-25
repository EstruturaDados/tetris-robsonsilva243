#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da peça
typedef struct {
    char nome;
    int id;
} Peca;

// Variáveis globais
int contadorID = 0;

// Função que gera uma peça nova
Peca gerarPeca() {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// ------------------- FILA CIRCULAR -------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

void iniciarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->tamanho++;
    }
}

Peca desenfileirar(Fila *f) {
    Peca p = {'X', -1};
    if (!filaVazia(f)) {
        p = f->itens[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->tamanho--;
    }
    return p;
}

// ------------------- PILHA -------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

void iniciarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = x;
    }
}

Peca desempilhar(Pilha *p) {
    Peca x = {'X', -1};
    if (!pilhaVazia(p)) {
        x = p->itens[p->topo--];
    }
    return x;
}

// ------------------- MOSTRAR ESTADO -------------------
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n================ ESTADO ATUAL ===============\n");

    printf("Fila: ");
    int i = f->inicio;
    for (int c = 0; c < f->tamanho; c++) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % TAM_FILA;
    }

    printf("\nPilha (Topo -> Base): ");
    for (int j = p->topo; j >= 0; j--) {
        printf("[%c %d] ", p->itens[j].nome, p->itens[j].id);
    }

    printf("\n=============================================\n");
}

// ------------------- TROCA DE 1 PEÇA -------------------
void trocarUma(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNão tem como trocar.\n");
        return;
    }

    int posFila = f->inicio;
    Peca temp = f->itens[posFila];
    f->itens[posFila] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nPeças trocadas com sucesso!\n");
}

// ------------------- TROCA DE 3 PEÇAS -------------------
void trocarTres(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("\nNão é possível fazer troca múltipla.\n");
        return;
    }

    int posFila = f->inicio;

    for (int i = 0; i < 3; i++) {
        Peca temp = f->itens[(posFila + i) % TAM_FILA];
        f->itens[(posFila + i) % TAM_FILA] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\nTroca múltipla realizada com sucesso!\n");
}

// ------------------- PROGRAMA PRINCIPAL -------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    iniciarFila(&fila);
    iniciarPilha(&pilha);

    // Preencher fila inicialmente
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;

    do {
        mostrarEstado(&fila, &pilha);

        printf("\nCódigo    Ação\n");
        printf("1         Jogar peça da frente da fila\n");
        printf("2         Enviar peça da fila para a pilha de reserva\n");
        printf("3         Usar peça da pilha de reserva\n");
        printf("4         Trocar peça da frente da fila com o topo da pilha\n");
        printf("5         Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0         Sair\n");

        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                if (!filaVazia(&fila)) {
                    Peca p = desenfileirar(&fila);
                    printf("\nPeça jogada: [%c %d]\n", p.nome, p.id);
                    enfileirar(&fila, gerarPeca());
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca p = desenfileirar(&fila);
                    empilhar(&pilha, p);
                    printf("\nPeça reservada!\n");
                    enfileirar(&fila, gerarPeca());
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca p = desempilhar(&pilha);
                    printf("\nPeça usada da pilha: [%c %d]\n", p.nome, p.id);
                }
                break;
            }

            case 4:
                trocarUma(&fila, &pilha);
                break;

            case 5:
                trocarTres(&fila, &pilha);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
