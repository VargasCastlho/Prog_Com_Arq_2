#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

///Dupla: Fernando Castilho e Thayná Marins

using namespace std;

typedef struct ocorrencia {
    int arquivo; // posicao do arquivo na lista de arquivos processados
    vector<int> linhas; // linhas onde a palavra foi encontrada
} Ocorrencia;

typedef struct palavra {
    string palavra; //palavra em questao
    vector<Ocorrencia> ocorrenciasP; // lista contendo informaçoes da ocorrencia de uma palavra em um arquivo
    // vector<int> ocorrencias; //antigo trabalho
} Palavra;

typedef struct indice {
    vector<string> arquivos; // lista contendo os nomes dos arquivos texto ja processados
    vector<Palavra> listaPalavras; //lista contendo todas as palavras ja encontrada
} Indice;



bool palavraJaexiste(string buscaPalavra, Indice &lista) {
    for (auto &p: lista.listaPalavras) {
        if (p.palavra.compare(buscaPalavra) == 0)
            return true;
    }
    return false;
}

int buscarIndex(string buscaPalavra, Indice &lista) {
    int cont = 0;
    for (auto &p: lista.listaPalavras) {
        if (p.palavra.compare(buscaPalavra) == 0)
            return cont;
        cont++;
    }
    return -1;
}

bool verificaCaracter(string palavra, string c) {
    if (palavra.substr(palavra.length() - 1).compare(c) == 0)
        return true;
    return false;
}

string verificaFinalPalavra(string palavra) {
    string p = palavra.substr(0, palavra.size());

    if (verificaCaracter(p, ".") || verificaCaracter(p, ",") || verificaCaracter(p, "!") || verificaCaracter(p, "?"))
        return p.substr(0, palavra.size() - 1);
    return p;
}

int verificaSeArquivoFoiProcessado(string nomeArquivo, Indice lista) {
    int cont = 0;
    for (string p: lista.arquivos) {
        if (p.compare(nomeArquivo) == 0)
            return cont;
        cont++;
    }
    return 0;
}

int verificaOcorrencia(string nomeArquivo, Indice lista, int index) {
    int cont = 0;
    int indexArquivo = verificaSeArquivoFoiProcessado(nomeArquivo, lista);
    for (Ocorrencia o: lista.listaPalavras[index].ocorrenciasP) {
        if (o.arquivo == indexArquivo)
            return cont;
        cont++;
    }
    return -1;
}

//adicionar ocorrencia
void adicionaPalavra(int linha, Indice &lista, string nomeArquivo, int index) {
    int indexOco = verificaOcorrencia(nomeArquivo, lista, index);
    if (indexOco != -1) {
        lista.listaPalavras[index].ocorrenciasP[indexOco].linhas.push_back(linha);
    } else {
        Ocorrencia ocorrencia1;
        ocorrencia1.arquivo = verificaSeArquivoFoiProcessado(nomeArquivo, lista);
        ocorrencia1.linhas.push_back(linha);
        lista.listaPalavras[index].ocorrenciasP.push_back(ocorrencia1);
    }

}

// insere uma palavra em ordem alfabética na lista

void inserePalavraOrdem(string p, int linha, Indice &lista, string nomeArquivo) {
    Palavra palavra1;
    Ocorrencia ocorrencia1;
    ocorrencia1.arquivo = verificaSeArquivoFoiProcessado(nomeArquivo, lista);
    ocorrencia1.linhas.push_back(linha);
    palavra1.palavra = p;
    palavra1.ocorrenciasP.push_back(ocorrencia1);


    int posicao = 0; //posição onde a palavra deve entrar

    while (posicao < lista.listaPalavras.size() &&
           (palavra1.palavra > lista.listaPalavras[posicao].palavra)) //se não chegou na posisção correta, continua
        posicao++;

    lista.listaPalavras.insert(lista.listaPalavras.begin() + posicao, palavra1);//insere na posição correta
}

void lerArquivo(string nomeArquivo, Indice &lista) {
    if (verificaSeArquivoFoiProcessado(nomeArquivo, lista) != 0) {
        cout << "Arquivo já processado!" << endl;
    } else {
        lista.arquivos.push_back(nomeArquivo);
        string linha;
        fstream arq(nomeArquivo, fstream::in);
        int cont = 0;
        while (arq.eof() == 0) {
            std::getline(arq, linha);
            string palavraTexto;
            stringstream ss(linha);

            while (getline(ss, palavraTexto, ' ')) {
                palavraTexto = verificaFinalPalavra(palavraTexto);
                if (palavraJaexiste(palavraTexto, lista)) {
                    int index = buscarIndex(palavraTexto, lista);
                    adicionaPalavra(cont, lista, nomeArquivo, index);
                } else {
                    inserePalavraOrdem(palavraTexto, cont, lista, nomeArquivo);
                }
            }
            cont++;
        }
        arq.close();
    }
}


///Trabalho parte 2

/// case 2

void escreveOcorrencia(FILE *arq, Ocorrencia ocorrencia) {
    ///escreve o numero do arquivo binario (occorencia int arquivo)
    fwrite(&ocorrencia.arquivo, sizeof(int), 1, arq);
    ///qtde de vezes que a palavra apareceu no arquivo
    int oc = ocorrencia.linhas.size();
    fwrite(&oc, sizeof(int), 1, arq);
    /// escreve as linhas onde a palavra apareceu neste arquivo
    fwrite(ocorrencia.linhas.data(), sizeof(int), ocorrencia.linhas.size(), arq);

}

void escrevePalavra(FILE *arq, Palavra palavra) {
    ///escreve qtde de letras da palavra;
    int caracteres = palavra.palavra.size() + 1;
    fwrite(&caracteres, sizeof(int), 1, arq);

    ///escreve no arq binario as letras da palavra
    fwrite(palavra.palavra.c_str(), caracteres, 1, arq);

    /// ocorrencias total
    int octotal = palavra.ocorrenciasP.size();
    fwrite(&octotal, sizeof(int), 1, arq);

    ///para cada ocorrecencia
    for (int i = 0; i < palavra.ocorrenciasP.size(); i++) {
        escreveOcorrencia(arq, palavra.ocorrenciasP[i]);
    }


}

void escreveNomeArquivo(FILE *arq, string nomeArquivo) {
    ///escreve a qtde de letras do nome
    int caracteres = nomeArquivo.size() + 1;
    fwrite(&caracteres, sizeof(int), 1, arq);
    ///escreve o nome no arq binario
    fwrite(nomeArquivo.c_str(), caracteres, 1, arq);


}

void salvarIndice(Indice &ind) {
    ///abrir arq binario
    FILE *arq;
    arq = fopen("indice.dat", "wb");
    ///escreve a qtde de arquivos texto processados
    int size = ind.arquivos.size();
    fwrite(&size, sizeof(int), 1, arq);
    ///para cada arquivo de ind.arquivo
    for (int i = 0; i < ind.arquivos.size(); i++) {
        escreveNomeArquivo(arq, ind.arquivos[i]);

    }
    /// escreve a qtde de palavras
    int qtdePalavras = ind.listaPalavras.size();
    fwrite(&qtdePalavras, sizeof(int), 1, arq);

    ///para cada palavra de ind.palavras
    for (int j = 0; j < ind.listaPalavras.size(); j++) {
        escrevePalavra(arq, ind.listaPalavras[j]);
    }

    ///fechar arq
    fclose(arq);
}

///case 3


void lerOcorrencia(FILE *arq, Ocorrencia &ocorrencia) {
    ///ler o numero do arquivo binario (ocorrencia int arquivo)
    fread(&ocorrencia.arquivo, sizeof(int), 1, arq);

    /// ocorrencia no arquivo
    int oc;
    fread(&oc, sizeof(int), 1, arq);
    ocorrencia.linhas.resize(oc);

    /// ler as linhas onde a palavra apareceu neste arquivo
    fread(ocorrencia.linhas.data(), sizeof(int), oc, arq);


}

void lerPalavra(FILE *arq, Indice &ind) {
    Palavra p;
    ///lendo a qtde de letras da palavra
    int qtdletraPal;
    fread(&qtdletraPal, sizeof(int), 1, arq);

    ///ler no arq binario as letras da palavra
    char letras[qtdletraPal];
    fread(letras, qtdletraPal, 1, arq);

    p.palavra.assign(letras);

    ///lendo a ocorrencia total
    int oct;
    fread(&oct, sizeof(int), 1, arq);
    p.ocorrenciasP.resize(oct);

    ///ler para cada ocorrecencia
    for (int i = 0; i < oct; i++) {
        lerOcorrencia(arq, p.ocorrenciasP[i]);
    }

    ind.listaPalavras.push_back(p);

}

void lerNomeArquivo(FILE *arq, Indice &ind) {
    ///ler a qtde de letras do nome
    int letrasqtd;
    fread(&letrasqtd, sizeof(int), 1, arq);

    ///ler o nome no arq binario
    char nome[letrasqtd];
    fread(nome, letrasqtd, 1, arq);

    ind.arquivos.push_back(nome);

}

void lerIndice(Indice &ind) {
    ///liberar indice;
    ind.arquivos.clear();
    ind.listaPalavras.clear();

    /// abrir o arq binario
    FILE *arq;
    arq = fopen("indice.dat", "rb");

    /// ler qtd de arquivos do indice
    int qtdArq;
    fread(&qtdArq, sizeof(int), 1, arq);

    /// para cada arquivo
    for (int i = 0; i < qtdArq; i++) {
        lerNomeArquivo(arq, ind);
    }

    /// ler a qtde de palavras do indice
    int qtdPal;
    fread(&qtdPal, sizeof(int), 1, arq);

    ///para cada palavra
    for (int j = 0; j < qtdPal; j++) {
        lerPalavra(arq, ind);
    }
    ///fechar arq
    fclose(arq);
}

void mostrarPalavrasEmOrdem(Indice ind) {
    cout << "Palavras que compoe este indice: " << endl;
    for (Palavra p : ind.listaPalavras) {
        cout << p.palavra << endl;
        for (Ocorrencia o : p.ocorrenciasP) {
            cout << "Numero do arquivo: " << o.arquivo << endl;
            cout << "Qtd de vzs que a palavra apareceu no arquivo: " << o.linhas.size() << endl;
            cout << "Numeros das linhas: ";
            for (int l : o.linhas) {
                cout <<" "<< l;
            }
            cout << endl;
        }
    }
    cout << endl;
}

void mostrarNomesArquivosIndice(vector<string> nomes) {
    cout << "Nomes dos arquivos que compoe este indice: ";
    for (string s: nomes) {
        cout <<" "<< s;
    }
    cout << endl;
}

void mostrarIndiceAtual(Indice ind) {
    mostrarNomesArquivosIndice(ind.arquivos);
    mostrarPalavrasEmOrdem(ind);
}

int main() {
    Indice ind;
    int resp = 0;
    string nameArquivo;
    while (resp != -1) {
        cout << endl << "<1>- Criar indice para um arquivo texto" << endl << "<2>- Salvar indice atual" << endl
             << "<3>- Ler um indice" << endl << "<4>- Mostrar as informacoes de um Indice" << endl << "<5>- Encerrar"
             << endl;
        cout << "Resp:";
        cin >> resp;
        string nomeA;

        switch (resp) {
            case 1:
                cout << "Nome do arquivo texto:" << endl;
                cin >> nameArquivo;
                lerArquivo(nameArquivo, ind);
                break;
            case 2:
                salvarIndice(ind);
                cout << "Indice Salvo!" << endl;
                break;
            case 3:
                lerIndice(ind);
                break;
            case 4:
                mostrarIndiceAtual(ind);
                break;
            case 5:
                cout << "Voce saiu do programa!" << endl;
                resp = -1;
                break;
            default:
                cout << "Opcao digitada nao reconhecida. Tente novamente" << endl;
        }

    }

    return 0;
}