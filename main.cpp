/*
 * CABEÇALHO OBRIGATÓRIO
 * Membros da Equipe:
 * - Matheus Fellipe Araujo Marques
 * - Henrique Furtini
 *
 * Tema do Trabalho:
 * - Cadastro de Frutas Tropicais
 */
#include <iostream>
#include <fstream> // do arquivo CSV
#include <string>
#include <sstream> //Melhoria na manipulação do arquivo csv
#include <limits>  //Limpeza de buffer e memória
using namespace std;
// Definição da estrutura
struct Frutas{
    int id;
    int calorias;
    string nome;
    string continente;
    string coloracao;
};
// FUNÇÕES USADAS
// Funções de Gerenciamento do Vetor
void adicionar_elementos(Frutas*& vetor, int& capacidade);
void carregar(Frutas*& vetor, int& n_elementos, int& capacidade);
void gravarArquivo(Frutas* vetor, int n_elementos);
// Funções do Menu e CRUD
void exibirMenu();
void inserirElemento(Frutas*& vetor, int& n_elementos, int& capacidade, string& ordenadoPor);
void removerElemento(Frutas* vetor, int& n_elementos);
void buscarElemento(Frutas* vetor, int n_elementos, string& ordenadoPor);
void mostrarDados(Frutas* vetor, int n_elementos);
void ordenarDados(Frutas* vetor, int n_elementos, string& ordenadoPor);
// Funções Auxiliares (Impressão e Busca Linear)
void imprimirFruta(Frutas& fruta);
int buscarPorId_Linear(Frutas* vetor, int n_elementos, int id);
void imprimirVetor(Frutas* vetor, int inicio, int fim);
// Funções de Ordenação (QuickSort)
void swap(Frutas& a, Frutas& b);
int particionarPorId(Frutas* vetor, int inicio, int fim);
void quickSortPorId(Frutas* vetor, int inicio, int fim);
int particionarPorNome(Frutas* vetor, int inicio, int fim);
void quickSortPorNome(Frutas* vetor, int inicio, int fim);
// Funções de Busca (Binária)
int buscaBinariaPorId(Frutas* vetor, int n_elementos, int id);
int buscaBinariaPorNome(Frutas* vetor, int n_elementos, string nome);
// FUNÇÃO PRINCIPAL 
int main(){
    int capacidade_total = 40; // Tamanho inicial
    int n_elementos_vetor = 0; // Quantidade de itens realmente cadastrados

    // Flag para busca binária
    string ordenadoPor = "nenhum"; // "nenhum", "id", "nome"
    Frutas* vetor = new Frutas[capacidade_total];
    // Chama a função de carga
    carregar(vetor, n_elementos_vetor, capacidade_total);
    // Definicao de variavel para escolha de função
    int opcao;
    string salvarsaida;
    do {
        exibirMenu();
        cout << endl;
        cin >> opcao;

        //Tratamento de exceções com strings
        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcao = 0;
        }
        
        // Limpa o buffer do cin para evitar problemas com getline
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (opcao) {
            case 1:
                inserirElemento(vetor, n_elementos_vetor, capacidade_total, ordenadoPor);
                break;
            case 2:
                removerElemento(vetor, n_elementos_vetor);
                break;
            case 3:
                buscarElemento(vetor, n_elementos_vetor, ordenadoPor);
                break;
            case 4:
                mostrarDados(vetor, n_elementos_vetor);
                break;
            case 5:
                ordenarDados(vetor, n_elementos_vetor, ordenadoPor);
                break;
            case 6:
                gravarArquivo(vetor, n_elementos_vetor);
                break;
            case 7:
                cout << "Deseja salvar suas alterações? " << endl;
                cout << "S: Salvar" << endl << "N: Não salvar" << endl;
                cin >> salvarsaida;
                if (salvarsaida == "S" || salvarsaida == "s"){
                    gravarArquivo(vetor, n_elementos_vetor);
                    cout << "Saindo do programa..." << endl;
                } else if (salvarsaida == "N" || salvarsaida == "n"){
                    cout << "Saindo do programa..." << endl;
                } else{
                    cout << "Opção inválida... Tente novamente!" << endl;
                    opcao = 0;
                }
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                cin.clear();
                continue;
        }

    } while (opcao != 7);
    delete[] vetor;
    return 0;
}

//  IMPLEMENTAÇÃO DAS FUNÇÕES 
// Função de redimensionamento
void adicionar_elementos(Frutas*& vetor, int& capacidade){
    Frutas* novo_vetor = new Frutas[capacidade + 10];
    
    for (int i = 0; i < capacidade; i++){
        novo_vetor[i] = vetor[i];
    }
    
    delete[] vetor;
    vetor = novo_vetor;
    capacidade += 10;
}

//FUNÇÃO CARREGAR
void carregar(Frutas*& vetor, int& n_elementos, int& capacidade){
    fstream arquivo("frutastropicais.csv");
    if (!arquivo.is_open()) {
        cerr << "Erro: Não foi possivel abrir o arquivo frutastropicais.csv" << endl;
        return;
    }

    string linha;
    getline(arquivo, linha);   // Pula a linha 1

    // Loop principal: lê uma linha inteira de cada vez
    while(getline(arquivo, linha)){
        if (linha.empty()) {
            continue;
        }

        // Usa stringstream para "quebrar" a linha usando a vírgula
        stringstream ss(linha);
        string id_csv, nome_csv, continente_csv, calorias_csv, coloracao_csv;

        // Tenta ler cada campo separado por vírgula
        if (!getline(ss, id_csv, ',')) continue;
        if (!getline(ss, nome_csv, ',')) continue;
        if (!getline(ss, continente_csv, ',')) continue;
        if (!getline(ss, calorias_csv, ',')) continue;
        getline(ss, coloracao_csv); // O último campo
        
        // Se o vetor está cheio, redimensiona
        if (n_elementos == capacidade){
            adicionar_elementos(vetor, capacidade);
        }
        
        //Adiciona ao vetor
        try {
            vetor[n_elementos].id = stoi(id_csv); //"Stoi" converte os caracteres para int
            vetor[n_elementos].calorias = stoi(calorias_csv);
            vetor[n_elementos].nome = nome_csv;
            vetor[n_elementos].continente = continente_csv;
            vetor[n_elementos].coloracao = coloracao_csv;
            n_elementos++;

        } catch (const std::invalid_argument& e) {
            // Ignora linhas mal formatadas
        }
    }
    arquivo.close();
    cout << "Carga finalizada. " << n_elementos << " frutas carregadas." << endl;
}

// Salva o vetor de volta no arquivo
void gravarArquivo(Frutas* vetor, int n_elementos){
    ofstream arquivo("frutastropicais.csv");
    if (!arquivo.is_open()) {
        cerr << "Erro: Não foi possivel abrir o arquivo para gravação." << endl;
        return;
    }

    // Escreve a linha 1 como no arquivo original
    arquivo << "#ID,Nome da Fruta,Continente de Origem,Calorias (aprox. por 100g),Coloracao\n";
    for (int i = 0; i < n_elementos; i++){
        arquivo << vetor[i].id << ","
                << vetor[i].nome << ","
                << vetor[i].continente << ","
                << vetor[i].calorias << ","
                << vetor[i].coloracao << "\n";
    }
    arquivo.close();
    cout << "Dados gravados com sucesso no arquivo 'frutastropicais.csv'." << endl;
}


void exibirMenu(){
    //Exibe menu para UX
    cout << "\n---- SISTEMA DE CADASTRO DE FRUTAS ----" << endl;
    cout << "1. Inserir nova fruta" << endl;
    cout << "2. Remover fruta (por ID)" << endl;
    cout << "3. Buscar fruta" << endl;
    cout << "4. Mostrar dados" << endl;
    cout << "5. Ordenar dados" << endl;
    cout << "6. Salvar alterações no arquivo" << endl;
    cout << "7. Sair" << endl;
    cout << "Escolha uma opção: ";
}

// Inserir elemento
void inserirElemento(Frutas*& vetor, int& n_elementos, int& capacidade, string& ordenadoPor){
    // Verifica se precisa redimensionar ANTES de inserir
    if (n_elementos == capacidade){
        adicionar_elementos(vetor, capacidade);
        cout << "Vetor redimensionado. Nova capacidade: " << capacidade << endl;
    }

    Frutas novaFruta;
    cout << "Digite o ID: ";
    cin >> novaFruta.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar o buffer

    cout << "Digite o Nome: ";
    getline(cin, novaFruta.nome);

    cout << "Digite a Região de Origem (Continente): ";
    getline(cin, novaFruta.continente);

    cout << "Digite as Calorias: ";
    cin >> novaFruta.calorias;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Digite a Coloração: ";
    getline(cin, novaFruta.coloracao);

    vetor[n_elementos] = novaFruta;
    n_elementos++;

    // Inserir no final quebra a ordenação
    ordenadoPor = "nenhum"; 
    
    cout << "Fruta '" << "-" << novaFruta.nome <<"-"<< "' inserida com sucesso." << endl;
}

// Remove um elemento
void removerElemento(Frutas* vetor, int& n_elementos){
    int idRemover;
    cout << "Digite o ID da fruta a ser removida: ";
    cin >> idRemover;

    int indice = buscarPorId_Linear(vetor, n_elementos, idRemover);

    if (indice == -1 ){
        cout << "Erro: Fruta com ID " << idRemover << " não encontrada." << endl;
        return;
    } 

    // Deslocamento físico dos elementos
    for (int i = indice; i < n_elementos - 1; i++){
        vetor[i] = vetor[i + 1];
    }

    n_elementos--; // Diminui o número total de elementos
    cout << "Fruta removida com sucesso." << endl;
}

// Menu de busca
void buscarElemento(Frutas* vetor, int n_elementos, string& ordenadoPor){
    int opcaoBusca;
    cout << "Buscar por:" << endl;
    cout << "1. ID (requer ordenação por ID)" << endl;
    cout << "2. Nome (requer ordenação por Nome)" << endl;
    cout << "Opção: ";
    cin >> opcaoBusca;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int indice = -1;
    if (opcaoBusca == 1){
        if (ordenadoPor != "id"){
            cout << "Erro: Os dados não estão ordenados por ID. Use a Opção 5." << endl;
            return;
        }
        int idBusca;
        cout << "Digite o ID a buscar: ";
        cin >> idBusca;
        indice = buscaBinariaPorId(vetor, n_elementos, idBusca);

    } else if (opcaoBusca == 2){
        if (ordenadoPor != "nome"){
            cout << "Erro: Os dados não estao ordenados por Nome. Use a Opção 5." << endl;
            return;
        }
        string nomeBusca;
        cout << "Digite o Nome a buscar: ";
        getline(cin, nomeBusca);
        indice = buscaBinariaPorNome(vetor, n_elementos, nomeBusca);

    } else {
        cout << "Opção de busca invalida." << endl;
        return;
    }

    if (indice != -1){
        cout << "Elemento encontrado:" << endl;
        imprimirFruta(vetor[indice]);
    } else {
        cout << "Elemento não encontrado." << endl;
    }
}

// Menu para mostrar dados
void mostrarDados(Frutas* vetor, int n_elementos){
    int opcao;
    cout << "1. Mostrar todos os dados" << endl;
    cout << "2. Mostrar um trecho" << endl;
    cout << "Opção: ";
    cin >> opcao;

    if (opcao == 1){
        cout << "--- Exibindo " << n_elementos << " frutas ---" << endl;
        imprimirVetor(vetor, 0, n_elementos);
    } else if (opcao == 2){
        int inicio, fim;
        cout << "Digite o indice inicial (inclusive, comeca em 0): ";
        cin >> inicio;
        cout << "Digite o indice final (exclusive): ";
        cin >> fim;

        if (inicio < 0 || fim > n_elementos || inicio >= fim){
            cout << "Intervalo invalido." << endl;
        } else {
            cout << "--- Exibindo frutas de " << inicio << " a " << fim - 1 << " ---" << endl;
            imprimirVetor(vetor, inicio, fim);
        }
    } else {
        cout << "Opção invalida." << endl;
    }
}

// Menu de ordenação
void ordenarDados(Frutas* vetor, int n_elementos, string& ordenadoPor){
    int ordenar;
    cout << "Ordenar por:" << endl;
    cout << "1. ID" << endl;
    cout << "2. Nome" << endl;
    cout << "Opção: ";
    cin >> ordenar;
    //Opções de ordenação e tratamento de exceção
    if (ordenar == 1){
        quickSortPorId(vetor, 0, n_elementos - 1);
        ordenadoPor = "id";
        cout << "Vetor ordenado por ID." << endl;
    } else if (ordenar == 2){
        quickSortPorNome(vetor, 0, n_elementos - 1);
        ordenadoPor = "nome";
        cout << "Vetor ordenado por Nome." << endl;
    } else {
        cout << "Opção invalida." << endl;
    }
}

// FUNÇÕES AUXILIARES 
void imprimirFruta(Frutas& fruta){
    cout << "--------------------" << endl;
    cout << "ID: " << fruta.id << endl;
    cout << "Nome: " << fruta.nome << endl;
    cout << "Continente: " << fruta.continente << endl;
    cout << "Calorias: " << fruta.calorias << endl;
    cout << "Coloração: " << fruta.coloracao << endl;
}
// Imprime na ordem em que estão armazenados
void imprimirVetor(Frutas* vetor, int inicio, int fim){
    for (int i = inicio; i < fim; i++){
        imprimirFruta(vetor[i]);
    }
    cout << "--------------------" << endl;
}
// Busca linear (usada para remoção, -não exige ordenação-)
int buscarPorId_Linear(Frutas* vetor, int n_elementos, int id){
    for (int i = 0; i < n_elementos; i++){
        if (vetor[i].id == id){
            return i; 
        }
    }
    return -1;
}

// FUNÇÕES DE ORDENAÇÃO (QUICKSORT)
void swap(Frutas& a, Frutas& b){
    Frutas temp = a;
    a = b;
    b = temp;
}
// QuickSort por ID
int particionarPorId(Frutas* vetor, int inicio, int fim){
    int pivo = vetor[fim].id;
    int i = (inicio - 1);
    for (int j = inicio; j <= fim - 1; j++){
        if (vetor[j].id <= pivo){
            i++;
            swap(vetor[i], vetor[j]);
        }
    }
    swap(vetor[i + 1], vetor[fim]);
    return (i + 1);
}
void quickSortPorId(Frutas* vetor, int inicio, int fim){
    if (inicio < fim){
        int pi = particionarPorId(vetor, inicio, fim);
        quickSortPorId(vetor, inicio, pi - 1);
        quickSortPorId(vetor, pi + 1, fim);
    }
}

// QuickSort por Nome
int particionarPorNome(Frutas* vetor, int inicio, int fim){
    string pivo = vetor[fim].nome;
    int i = (inicio - 1);
    for (int j = inicio; j <= fim - 1; j++){
        if (vetor[j].nome <= pivo){
            i++;
            swap(vetor[i], vetor[j]);
        }
    }
    swap(vetor[i + 1], vetor[fim]);
    return (i + 1);
}
void quickSortPorNome(Frutas* vetor, int inicio, int fim){
    if (inicio < fim){
        int pi = particionarPorNome(vetor, inicio, fim);
        quickSortPorNome(vetor, inicio, pi - 1);
        quickSortPorNome(vetor, pi + 1, fim);
    }
}


// FUNÇÕES DE BUSCA BINÁRIA 
int buscaBinariaPorId(Frutas* vetor, int n_elementos, int id){
    int esquerda = 0;
    int direita = n_elementos - 1;
    while (esquerda <= direita){
        int meio = esquerda + (direita - esquerda) / 2;
        if (vetor[meio].id == id)
            return meio;
        if (vetor[meio].id < id)
            esquerda = meio + 1;
        else
            direita = meio - 1; 
    }
    return -1; 
}
int buscaBinariaPorNome(Frutas* vetor, int n_elementos, string nome){
    int esquerda = 0;
    int direita = n_elementos - 1;
    while (esquerda <= direita){
        int meio = esquerda + (direita - esquerda) / 2;
        if (vetor[meio].nome == nome)
            return meio;
        if (vetor[meio].nome < nome)
            esquerda = meio + 1; 
        else
            direita = meio - 1; 
    }
    return -1;
}