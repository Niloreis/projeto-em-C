#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 100
#define NOME_SIZE 50
#define ARQUIVO_ALUNOS "alunos.txt"

// Estrutura para armazenar os dados do aluno
typedef struct {
    int matricula;
    char nome[NOME_SIZE];
    int idade;
} Aluno;

// Array para armazenar os alunos (alocação estática, mas com controle de tamanho)
Aluno alunos[MAX_ALUNOS];
int numAlunos = 0;

// Protótipos das funções
void adicionarAluno();
void listarAlunos();
void editarAluno();
void apagarAluno();
int buscarAluno(int matricula);
void salvarAlunos();
void carregarAlunos();

int main() {
    int opcao;

    carregarAlunos(); 

    do {
        printf("\n--- Sistema de Cadastro de Alunos ---\n");
        printf("1. Adicionar Aluno\n");
        printf("2. Listar Alunos\n");
        printf("3. Editar Aluno\n");
        printf("4. Apagar Aluno\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                adicionarAluno();
                break;
            case 2:
                listarAlunos();
                break;
            case 3:
                editarAluno();
                break;
            case 4:
                apagarAluno();
                break;
            case 0:
                salvarAlunos(); // Salva os dados dos alunos ao sair do programa
                printf("Saindo do sistema. Ate a proxima!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

void adicionarAluno() {
    if (numAlunos < MAX_ALUNOS) {
        Aluno novoAluno;
        printf("\n--- Adicionar Novo Aluno ---\n");
        printf("Matricula: ");
        scanf("%d", &novoAluno.matricula);
        getchar(); 

        // Verificar se a matrícula já existe
        if (buscarAluno(novoAluno.matricula) != -1) {
            printf("Erro: Matricula ja cadastrada.\n");
            return;
        }

        printf("Nome: ");
        fgets(novoAluno.nome, NOME_SIZE, stdin);
        novoAluno.nome[strcspn(novoAluno.nome, "\n")] = 0; 

        printf("Idade: ");
        scanf("%d", &novoAluno.idade);
        getchar(); 

        alunos[numAlunos++] = novoAluno;
        printf("Aluno cadastrado com sucesso!\n");
    } else {
        printf("Erro: Limite maximo de alunos atingido.\n");
    }
}

void listarAlunos() {
    if (numAlunos > 0) {
        printf("\n--- Lista de Alunos Cadastrados ---\n");
        for (int i = 0; i < numAlunos; i++) {
            printf("Matricula: %d\n", alunos[i].matricula);
            printf("Nome: %s\n", alunos[i].nome);
            printf("Idade: %d\n", alunos[i].idade);
            printf("-------------------------\n");
        }
    } else {
        printf("Nenhum aluno cadastrado.\n");
    }
}

void editarAluno() {
    int matriculaEditar;
    printf("\n--- Editar Aluno ---\n");
    printf("Digite a matricula do aluno que deseja editar: ");
    scanf("%d", &matriculaEditar);
    getchar(); 

    int indice = buscarAluno(matriculaEditar);

    if (indice != -1) {
        printf("Aluno encontrado:\n");
        printf("Matricula: %d\n", alunos[indice].matricula);
        printf("Nome atual: %s\n", alunos[indice].nome);
        printf("Idade atual: %d\n", alunos[indice].idade);

        printf("\n--- Novas informações ---\n");
        printf("Novo nome (deixe em branco para manter): ");
        char novoNome[NOME_SIZE];
        fgets(novoNome, NOME_SIZE, stdin);
        novoNome[strcspn(novoNome, "\n")] = 0;
        if (strlen(novoNome) > 0) {
            strcpy(alunos[indice].nome, novoNome);
        }

        printf("Nova idade (digite -1 para manter): ");
        int novaIdade;
        scanf("%d", &novaIdade);
        getchar();
        if (novaIdade != -1) {
            alunos[indice].idade = novaIdade;
        }

        printf("Informacoes do aluno atualizadas com sucesso!\n");
    } else {
        printf("Erro: Aluno com matricula %d nao encontrado.\n", matriculaEditar);
    }
}

void apagarAluno() {
    int matriculaApagar;
    printf("\n--- Apagar Aluno ---\n");
    printf("Digite a matricula do aluno que deseja apagar: ");
    scanf("%d", &matriculaApagar);
    getchar(); 

    int indice = buscarAluno(matriculaApagar);

    if (indice != -1) {
        // Deslocar os alunos subsequentes para preencher o espaço do aluno apagado
        for (int i = indice; i < numAlunos - 1; i++) {
            alunos[i] = alunos[i + 1];
        }
        numAlunos--;
        printf("Aluno com matricula %d apagado com sucesso!\n", matriculaApagar);
    } else {
        printf("Erro: Aluno com matricula %d nao encontrado.\n", matriculaApagar);
    }
}

// Função para buscar um aluno pela matrícula e retornar seu índice no array
int buscarAluno(int matricula) {
    for (int i = 0; i < numAlunos; i++) {
        if (alunos[i].matricula == matricula) {
            return i; 
        }
    }
    return -1; 
}

// Função para salvar os dados dos alunos no arquivo
void salvarAlunos() {
    FILE *arquivo = fopen(ARQUIVO_ALUNOS, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    for (int i = 0; i < numAlunos; i++) {
        fprintf(arquivo, "%d %s %d\n", alunos[i].matricula, alunos[i].nome, alunos[i].idade);
    }

    fclose(arquivo);
    printf("Dados dos alunos salvos com sucesso!\n");
}

// Função para carregar os dados dos alunos do arquivo
void carregarAlunos() {
    FILE *arquivo = fopen(ARQUIVO_ALUNOS, "r");
    if (arquivo == NULL) {
        printf("Arquivo de alunos nao encontrado. Iniciando com lista vazia.\n");
        return;
    }

    while (fscanf(arquivo, "%d %s %d", &alunos[numAlunos].matricula, alunos[numAlunos].nome, &alunos[numAlunos].idade) == 3) {
        numAlunos++;
        if (numAlunos >= MAX_ALUNOS) {
            printf("Aviso: Limite maximo de alunos carregados.\n");
            break;
        }
    }

    fclose(arquivo);
    printf("Dados dos alunos carregados com sucesso!\n");
}
