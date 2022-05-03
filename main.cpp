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
    fstream arq("C://Users//thayn//Documents//UFF//4_PERIODO//Prog_com_arquivos//prog_tab//a.txt", fstream::in);
    //fstream arq("C://Users//STI//CLionProjects//untitled3//a.txt", fstream::in);
    //fstream arq(nomeArquivo, fstream::in);
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
    //fseek(arq, 0, SEEK_END);
    fwrite(&size, sizeof(int), 1, arq);
    for(auto p : lista) {

        int caracteres = p.palavra.size()+1;
        //fseek(arq, 0, SEEK_END);
        fwrite(&caracteres, sizeof(int), 1, arq);

        string palavraP;
        palavraP = p.palavra + '\0';
        fseek(arq, 0, SEEK_END);
        fwrite(palavraP.c_str(), caracteres, 1, arq);
        /*
        int soma = 0;
        for (int i = 0; i < p.ocorrencias.size(); i++) {
            soma += p.ocorrencias[i];
        }

       // fseek(arq, 0, SEEK_END);
        fwrite(&soma, sizeof(int), 1, arq);
        */

        // quantass x a palavra apareceu
        int aux = p.ocorrencias.size();
        fwrite(&aux, sizeof(int),1, arq);

        //linhas em que apareceu:
        fwrite(p.ocorrencias.data(), p.ocorrencias.size(),1, arq);

        /*
        for (int i = 0; i < p.ocorrencias.size(); i++) {
            if(p.ocorrencias[i]>0){
                fseek(arq, 0, SEEK_END);
                fwrite(&i, sizeof(int), 1, arq);
            }
        }*/
    }
    fclose(arq);
    /*
     * int totalp;
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
    fclose(aBin);
     * */


}

void utilizarIndice() {

    vector<palavra> lista;
    int totalp;
    palavra binpalavra;
    char paux[50];
  //  palavraBin binpalavra;

    FILE *aBin;
    aBin = fopen("indice.dat", "rb");




    fclose(aBin);

/*
    ifstream aBin;
    aBin.open("indice.dat", ios_base::in | ios_base::binary);

    //conseguiu ler o primeiro inteiro do arquivo
    aBin.read((char *) &totalp, sizeof(int));
    cout << "Quantidade total de palavras: " << totalp << endl;
*/
/*
 //tentativa de ler com struct
    while(aBin.eof()==0) {

        aBin.read((char *) &binpalavra.qtdCaracteres, sizeof(int));
        aBin.read((char *) &binpalavra.palavraB, sizeof(palavra));
        aBin.read((char *) &binpalavra.qtdOcorrencias, sizeof(int));
        for(int i = 0; i < binpalavra.qtdOcorrencias; i++){
            aBin.read((char *) &binpalavra.linhas[i], sizeof(int));

        }
        //nao sei direito como faria aqui pq as x uma plavra tem duas linhas então dois inteiros e as x um inteiro so (uma linha)

    }


    /*

    cout << caracteresBin <<" " << palavra <<" " << qtdOcorrencia;

*/

/*
    // leitura sem struct, quando vai ler a palavra da erro de acesso de memoria (<error: Cannot access memory at address 0xeb1efff820>)
    while(aBin.eof()==0){

        aBin.read((char *) &caracteresBin, sizeof(int));
        aBin.read((char *) &palavra, sizeof(palavra));
        aBin.read((char *) &qtdOcorrencia, sizeof(int));

        cout << caracteresBin <<" " << palavra <<" " << qtdOcorrencia;

        for(int i = 0; i < qtdOcorrencia; i++){
            aBin.read((char *) &linha, sizeof(int));
            cout << " "<< linha;
        }
        cout << endl;
    }
*/



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