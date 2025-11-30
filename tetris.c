#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// =============================================================// DEFINIÇÕES E ESTRUTURAS// =============================================================

#define TAM_FILA 5
#define TAM_PILHA 3

// --- 🧩 ESTRUTURAS ---
typedef struct {

    char tipo; // 'I', 'J', 'L', 'O', 'S', 'T', 'Z'
    int id;

} Peca;

typedef struct {

    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int total;

} FilaCircular;

typedef struct {

    Peca itens[TAM_PILHA];
    int topo;
} Pilha; 

// Variável global para gerar IDs sequenciais

int id_sequencial = 1;

// --- FUNÇÕES AUXILIARES ---
// Gera uma peça aleatória
Peca gerarPeca(void) {
    Peca p;
    char tipos[] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};
    p.tipo = tipos[rand() % 7];
    p.id = id_sequencial++;
    return p;
}

// --- FUNÇÕES DA FILA ---

void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;

}
int filaVazia(FilaCircular *f) { return (f->total == 0); }
int filaCheia(FilaCircular *f) { return (f->total == TAM_FILA); }
void enqueue(FilaCircular *f, Peca p) {

    if (filaCheia(f)) return;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA; // Lógica Circular
    f->total++;

}Peca dequeue(FilaCircular *f) {

    Peca p = {' ', 0}; // Peça vazia de erro
    if (filaVazia(f)) return p;
    
    p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA; // Lógica Circular
    f->total--;

    return p;

}

// --- FUNÇÕES DA PILHA ---

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}int pilhaVazia(Pilha *p) { return (p->topo == -1); }
int pilhaCheia(Pilha *p) { return (p->topo == TAM_PILHA - 1); }
int push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return 0; // Erro

    p->topo++;
    p->itens[p->topo] = peca;

    return 1; // Sucesso

}Peca pop(Pilha *p) {

    Peca peca = {' ', 0};

    if (pilhaVazia(p)) return peca;
    peca = p->itens[p->topo];
    p->topo--;

    return peca;

}
// --- FUNÇÃO DE VISUALIZAÇÃO ---

void mostrarEstado(FilaCircular f, Pilha p) {

    printf("\n================ TETRIS STACK ================\n");

    

    // Mostra Pilha (Reserva)

    printf("  [ PILHA (Reserva) ]    |   [ FILA (Proximas) ]\n");
    printf("       Topo: %d/%d         |      Inicio -> Fim\n", p.topo + 1, TAM_PILHA); 

    int i_pilha = p.topo;
    int i_fila = f.inicio;

    // Vamos imprimir linha por linha para comparar visualmente

    for(int i = 0; i < TAM_FILA; i++) {

        // Lado da Pilha

        if (i < TAM_PILHA && i_pilha >= 0) {
            printf("      [%c - #%02d]        |", p.itens[i_pilha].tipo, p.itens[i_pilha].id);
            i_pilha--;
        } else if (i < TAM_PILHA) {
            printf("      [ VAZIO ]        |");
        } else {
            printf("                       |");

        }

        // Lado da Fila

        if (i < f.total) {

            printf("   --> [%c - #%02d]", f.itens[i_fila].tipo, f.itens[i_fila].id);

            i_fila = (i_fila + 1) % TAM_FILA;
        }

        printf("\n");
    }

    printf("==============================================\n");

}
int main() {


    srand(time(NULL)); // Inicializa aleatoriedade
    
    // Variáveis do sistema
    FilaCircular fila;
    Pilha pilha;
    int opcao;
    Peca pecaTemp, pecaAux;

    // Inicializações
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Enche a fila inicialmente (Nível Novato)
    for(int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        mostrarEstado(fila, pilha);

        printf("\n--- MENU DE ACOES ---\n");
        printf("1 - Jogar peca (Dequeue)\n");
        printf("2 - Reservar peca (Fila -> Pilha)\n");
        printf("3 - Usar peca reservada (Pop)\n");
        printf("4 - [MESTRE] Trocar Frente Fila <-> Topo Pilha\n");
        printf("5 - [MESTRE] Trocar 3 Primeiros Fila <-> 3 da Pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        // 🧩 Fila de Peças Futuras

        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                pecaTemp = dequeue(&fila);
                printf("\n>>> Voce JOGOU a peca [%c - #%d]!\n", pecaTemp.tipo, pecaTemp.id);
                // Regra: A cada remoção, insira uma nova peça
                enqueue(&fila, gerarPeca()); 
            }
        }

    // 🧠  Adição da Pilha de Reserva
        
        else if (opcao == 2) {
            if (pilhaCheia(&pilha)) {
                printf("\n[!] Pilha cheia! Nao pode reservar.\n");
            } else {
                pecaTemp = dequeue(&fila); // Tira da fila
                push(&pilha, pecaTemp);    // Põe na pilha
                printf("\n>>> Peca [%c - #%d] reservada!\n", pecaTemp.tipo, pecaTemp.id);
                // Regra: Repor a fila imediatamente
                enqueue(&fila, gerarPeca());
            }
        }
        else if (opcao == 3) {
            if (pilhaVazia(&pilha)) {
                printf("\n[!] Nenhuma peca na reserva!\n");
            } else {
                pecaTemp = pop(&pilha);
                printf("\n>>> Voce USOU a peca reservada [%c - #%d]!\n", pecaTemp.tipo, pecaTemp.id);
            }
        }


    // 🔄 Integração Estratégica entre Fila e Pilha
    

        else if (opcao == 4) {
            // Trocar a peça da frente da fila com o topo da pilha
            if (filaVazia(&fila) || pilhaVazia(&pilha)) {
                printf("\n[!] Erro: Precisa de itens na Fila e na Pilha para trocar.\n");
            } else {
                // Acesso direto aos arrays para troca (Swap)
                // Fila: fila.itens[fila.inicio]
                // Pilha: pilha.itens[pilha.topo]
                
                pecaAux = fila.itens[fila.inicio];
                fila.itens[fila.inicio] = pilha.itens[pilha.topo];
                pilha.itens[pilha.topo] = pecaAux;
                
                printf("\n>>> TROCA REALIZADA (1x1)!\n");
            }
        }
        else if (opcao == 5) {

            // Trocar os 3 primeiros da fila com as 3 peças da pilha
            // Verificação rigorosa

            if (pilha.topo != 2) { // 2 significa índice 0, 1, 2 (3 itens)
                printf("\n[!] Erro: A pilha deve estar CHEIA (3 itens) para esta troca.\n");
            } else {
                printf("\n>>> TROCA DE LOTE (3x3) REALIZADA!\n");
                
                // Iterar 3 vezes para trocar
                int idxFila = fila.inicio;
                int idxPilha = pilha.topo; // Começa do topo
                
                for(int i=0; i<3; i++) {
                    // Swap
                    pecaAux = fila.itens[idxFila];
                    fila.itens[idxFila] = pilha.itens[idxPilha];
                    pilha.itens[idxPilha] = pecaAux;
                    
                    // Avança índices
                    idxFila = (idxFila + 1) % TAM_FILA; // Avança circularmente na fila
                    idxPilha--; // Desce na pilha
                }
            }
        }

    } while (opcao != 0);

    printf("\nSaindo do Tetris Stack...\n");

    return 0;
}