#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//Dupla: Fernando Castilho e Thayná Marins

using namespace std;

typedef struct palavra{
    string palavra;
    vector<int> ocorrencias;
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
    palavra1.ocorrencias.push_back(linha);
    lista.push_back(palavra1);
}

void lerArquivo(string nomeArquivo, vector<palavra> &lista){
    lista.clear();
    string linha;
    fstream arq(nomeArquivo, fstream::in);
    int cont = 0;
    while(arq.eof()==0){
        std::getline(arq, linha);
        string palavraTexto;
        stringstream ss(linha);

        while(getline(ss, palavraTexto, ' ')){
            palavraTexto = verificaFinalPalavra(palavraTexto);
            if(palavraJaexiste(palavraTexto, lista)){
                int index = buscarIndex(palavraTexto, lista);
                lista[index].ocorrencias.push_back(cont);
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
        fwrite(&caracteres, sizeof(int), 1, arq);

        string palavraP;
        palavraP = p.palavra + '\0';
        fwrite(palavraP.c_str(), caracteres, 1, arq);

        // quantass x a palavra apareceu
        int aux = p.ocorrencias.size();
       // cout << aux << " ";
        fwrite(&aux, sizeof(int),1, arq);

        //linhas em que apareceu:
        fwrite(p.ocorrencias.data(), sizeof(int),p.ocorrencias.size(), arq);
    }
    fclose(arq);



}

void  buscaPalavra(string palavraB, vector<palavra> &lista){
    lista.begin();
    int avalia = 0;
    for (auto &p : lista) {
        if(p.palavra.compare(palavraB) == 0){
            cout << "Palavra encontrada :)!"<< endl;
            cout << "A palava apareceu " << p.ocorrencias.size() << " vez(es) no arquivo original." << " Linhas: ";
            for (int i = 0; i < p.ocorrencias.size(); ++i) {
                cout <<" "<< p.ocorrencias[i] <<" ";

            }
            cout << endl;
            avalia = 1;

        }

    }
    if(avalia==0){
        cout << "Palavra nao encontrada :(" << endl;
    }
}

void utilizarIndice() {

    vector<palavra> lista;
    int totalp, control=0;

    FILE *aBin;
    aBin = fopen("indice.dat", "rb");
    fseek(aBin, 0, SEEK_CUR);
    fread(&totalp, sizeof(int),1, aBin);
    cout << "Quantidade total de palavras: " << totalp << endl;

    do{
        palavra p;

        int c;
        int oc;

        fread(&c, sizeof(int),1, aBin); //caracteres da palavra

        char paux[c];
        fread(paux, c ,1, aBin);
        p.palavra.assign(paux);
        fread(&oc, sizeof(int),1, aBin); //ocorrencias
        p.ocorrencias.resize(oc);
        fread(p.ocorrencias.data(), sizeof(int), oc, aBin);
        cout << "Qtde. caracteres: " << c << " | Palavra: " << p.palavra << " | Num de ocorrencias: "<< p.ocorrencias.size() <<" | Linhas:";

        for (int i = 0; i < p.ocorrencias.size(); ++i) {
            cout <<" "<< p.ocorrencias[i] <<" ";

        }

        cout << endl;
        lista.push_back(p);



        control++;
    }while(control < totalp);
    fclose(aBin);


    string buscarPalavra;
    cout << "Qual palavra voce deseja pesquisar? "<< endl;
    cin >> buscarPalavra;


    buscaPalavra(buscarPalavra, lista);





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
                utilizarIndice();
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