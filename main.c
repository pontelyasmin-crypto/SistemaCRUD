#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[100];
    char cpf[12];
    int idade;
    char email[100];
} Pessoa;

void cadastrarPessoa() {
    Pessoa p;

    printf("Digite o nome da pessoa: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("Digite o CPF (11 dígitos): ");
    fgets(p.cpf, sizeof(p.cpf), stdin);
    p.cpf[strcspn(p.cpf, "\n")] = '\0';

    printf("Digite a idade: ");
    scanf("%d", &p.idade);
    getchar();  // limpa o ENTER

    printf("Digite o email: ");
    fgets(p.email, sizeof(p.email), stdin);
    p.email[strcspn(p.email, "\n")] = '\0';

    // ----- Abrindo o arquivo -----
    FILE *f = fopen("cadastros.bin", "ab");
    if (!f) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // ----- Gravando -----
    fwrite(&p, sizeof(Pessoa), 1, f);
    fclose(f);

    printf("\nCadastro salvo com sucesso!\n\n");
}

void listarPessoas() {
    FILE *f = fopen("cadastros.bin", "rb");
    if (!f) {
        printf("Nenhum cadastro encontrado.\n\n");
        return;
    }

    Pessoa p;
    printf("\n---- Lista de cadastros ----\n");

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        printf("\nNome : %s\n", p.nome);
        printf("CPF  : %s\n", p.cpf);
        printf("Idade: %d\n", p.idade);
        printf("Email: %s\n", p.email);
    }

    printf("\n----------------------------\n\n");

    fclose(f);
}

void listarPessoaCPF(char cpf[]) {
    FILE *f = fopen("cadastros.bin", "rb");
    if (!f) {
        printf("Nenhum cadastro encontrado.\n\n");
        return;
    }

    Pessoa p;
    printf("\n---- Busca por CPF ----\n");
  
    int achou = 0;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {    
            printf("\nNome : %s\n", p.nome);
            printf("CPF  : %s\n", p.cpf);
            printf("Idade: %d\n", p.idade);
            printf("Email: %s\n", p.email);
            achou = 1;
            break;
        }
    }

    if (!achou)
        printf("\nNenhum registro encontrado com esse CPF.\n");

    printf("\n----------------------------\n\n");

    fclose(f);
}

void editarPessoaCPF(char cpfBuscado[]) {
    FILE *f = fopen("cadastros.bin", "rb");
    if (!f) {
        printf("Nenhum cadastro encontrado.\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(f);
        return;
    }

    Pessoa p;
    int encontrado = 0;

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpfBuscado) == 0) {
            encontrado = 1;

            printf("\nCadastro encontrado! Digite os novos dados:\n");

            printf("Novo nome: ");
            fgets(p.nome, sizeof(p.nome), stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0';

            printf("Novo CPF (11 dígitos): ");
            fgets(p.cpf, sizeof(p.cpf), stdin);
            p.cpf[strcspn(p.cpf, "\n")] = '\0';

            printf("Nova idade: ");
            scanf("%d", &p.idade);
            getchar();

            printf("Novo email: ");
            fgets(p.email, sizeof(p.email), stdin);
            p.email[strcspn(p.email, "\n")] = '\0';

            printf("\nCadastro atualizado com sucesso!\n");
        }

        // grava o registro (atualizado ou não) no arquivo temporário
        fwrite(&p, sizeof(Pessoa), 1, temp);
    }

    fclose(f);
    fclose(temp);

    if (!encontrado) {
        printf("\nNenhum registro encontrado com esse CPF.\n");
        remove("temp.bin");
        return;
    }

    // substitui o arquivo antigo pelo novo
    remove("cadastros.bin");
    rename("temp.bin", "cadastros.bin");
}

void removerPessoaCPF(char cpfBuscado[]) {
    FILE *f = fopen("cadastros.bin", "rb");
    if (!f) {
        printf("Nenhum cadastro encontrado.\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(f);
        return;
    }

    Pessoa p;
    int encontrado = 0;

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        encontrado = 0;
        if (strcmp(p.cpf, cpfBuscado) == 0) {
            encontrado = 1;
            printf("\nRemovido com sucesso!\n");
        }

        // grava o registro (atualizado ou não) no arquivo temporário
        if(encontrado != 1){
            fwrite(&p, sizeof(Pessoa), 1, temp);
        }
    }

    fclose(f);
    fclose(temp);

    if (!encontrado) {
        printf("\nNenhum registro encontrado com esse CPF.\n");
        remove("temp.bin");
        return;
    }

    // substitui o arquivo antigo pelo novo
    remove("cadastros.bin");
    rename("temp.bin", "cadastros.bin");
}


int main() {
    int opcao;

    do {
        printf("=== MENU ===\n");
        printf("1 - Cadastrar pessoa\n");
        printf("2 - Listar pessoas\n");
        printf("3 - Buscar por CPF\n");
        printf("4 - Atualizar por CPF\n");
        printf("5 - Remover por CPF\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpa ENTER

        switch (opcao) {
            case 1:
                cadastrarPessoa();
                break;

            case 2:
                listarPessoas();
                break;
            case 3: {
                char cpf[11];
                printf("Digite o CPF para buscar: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                listarPessoaCPF(cpf);
                break;
            }
            case 4: {
                char cpf[11];
                printf("Digite o CPF para buscar: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                editarPessoaCPF(cpf);
                break;
            }
            case 5: {
                char cpf[11];
                printf("Digite o CPF para buscar: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                removerPessoaCPF(cpf);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n\n");
        }

    } while (opcao != 0);

    return 0;
}