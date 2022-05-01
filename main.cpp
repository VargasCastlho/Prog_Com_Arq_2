#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct palavra{
    string palavra;
    vector<int> ocorrencias = {0};
}palavra;



bool palavraJaexiste(string buscaPalavra, vector<palavra> &lista){
    lista.begin();
    for (auto &p : lista) {
        if(p.palavra.compare(buscaPalavra) == 0)
            return true;
    }
    return false;
}

int buscarIndex(string buscaPalavra, vector<palavra> &lista){
    int cont = 0;
    for (auto &p : lista) {
        if(p.palavra.compare(buscaPalavra) == 0)
            return cont;
        cont++;
    }
    return NULL;
}

string verificaFinalPalavra(string palavra){
    string p = palavra.substr(0, palavra.size());

    if(p.ends_with('.')||p.ends_with(',')||p.ends_with('!')||p.ends_with('?'))
        return p.substr(0, palavra.size()-1);
    return p;
}

void inserePalavraOrdem(string p, int linha, vector<palavra> &lista){
    palavra palavra1;
    palavra1.palavra = p;
    palavra1.ocorrencias.resize(palavra1.ocorrencias.capacity()+linha);
    palavra1.ocorrencias.insert(palavra1.ocorrencias.begin()+linha, 1);
    lista.push_back(palavra1);
}

void lerArquivo(string nomeArquivo, vector<palavra> &lista){
    string linha;

    fstream arq("C://Users//STI//CLionProjects//untitled3//a.txt", fstream::in);
    int cont = 0;
    while(arq.eof()==0){
        std::getline(arq, linha);
        string palavraTexto;
        stringstream ss(linha);
        while(getline(ss, palavraTexto, ' ')){
            palavraTexto = verificaFinalPalavra(palavraTexto);
            if(palavraJaexiste(palavraTexto, lista)){
                int index = buscarIndex(palavraTexto, lista);
                lista[index].ocorrencias.resize(lista[index].ocorrencias.capacity()+cont);
                ++lista[index].ocorrencias[cont];
            }
            else{
                inserePalavraOrdem(palavraTexto, cont, lista);
            }
        }
        cont++;
    }
    arq. close();
}

void escreverIndice(vector<palavra> &lista){
    FILE *arq;
    arq = fopen("indice.dat", "wb");
    int size = lista.size();
    fseek(arq, 0, SEEK_END);
    fwrite(&size, sizeof(int), 1, arq);
    for(auto p : lista) {
        int caracteres = p.palavra.size()+1;
        fseek(arq, 0, SEEK_END);
        fwrite(&caracteres, sizeof(int), 1, arq);

        string palavraP;
        palavraP = p.palavra + '\0';
        fseek(arq, 0, SEEK_END);
        fwrite(&palavraP, sizeof(string), 1, arq);

        int soma = 0;
        for (int i = 0; i < p.ocorrencias.size(); i++) {
            soma += p.ocorrencias[i];
        }
        fseek(arq, 0, SEEK_END);
        fwrite(&soma, sizeof(int), 1, arq);

        for (int i = 0; i < p.ocorrencias.size(); i++) {
            if(p.ocorrencias[i]>0){
                fseek(arq, 0, SEEK_END);
                fwrite(&i, sizeof(int), 1, arq);
            }
        }
    }
    fclose(arq);

    int totalp;
    FILE *aBin;
    aBin = fopen("indice.dat", "rb");
    fseek(aBin, 0, SEEK_SET);
    fread(&totalp, sizeof(int),1, aBin);
    cout << "Quantidade total de palavras: " << totalp << endl;

    int c;
    fseek(aBin, 0, SEEK_CUR);
    fread(&c, sizeof(int),1, aBin);
    cout << c << endl;

    string plv;
    fseek(aBin, 0, SEEK_CUR);
    fread(&plv, sizeof(plv),1, aBin);
    cout << plv << endl;
    fclose(arq);
}


void utilizarIndice() {
    vector<palavra> lista;
    string procurarPalavra;
    palavra p;
    int totalp;
    //a,b,c
    ifstream aBin;
    aBin.open("indice.dat", ios_base::in | ios_base::binary);
    aBin.read((char *) &totalp, sizeof(int));
    cout << "Quantidade total de palavras:" << totalp << endl;

    if (aBin.is_open()) {
        while (aBin.read((char *) &p, sizeof(palavra))) {
            cout << p.palavra << endl;


        }
        aBin.close();
    } else {
        cout << "Problemas ao abrir o arquivo!" << endl;
    }


    //d,e
    cout << "Qual palavra deseja pesquisar?" << endl;
    cin >> procurarPalavra;
    if (palavraJaexiste(procurarPalavra, lista)) {


    } else {
        cout << "Palavra nao encontrada." << endl;
    }

}

int main() {
    vector<palavra> lista;
    int resp = 0;
    string nameArquivo;
    while(resp != -1){
        cout << endl <<"<1>- Criar indice para um arquivo texto" << endl << "<2>- Utilizar um indice existente para realizar buscas por palavras" <<endl
             << "<3>- Encerrar o programa" << endl;
        cout << "Resp:";
        cin >> resp;

        switch(resp){
            case 1:
                cout << "Nome do arquivo texto:" <<endl;
                cin >> nameArquivo;
                lerArquivo(nameArquivo, lista);
                escreverIndice(lista);
                break;
            case 2:
                break;
            case 3:
                cout <<"Voce saiu do programa!" << endl;
                resp = -1;
                break;
            default:
                cout << "Opcao digitada nao reconhecida. Tente novamente"<< endl;
        }

    }










    return 0;
}
