#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//Dupla: Fernando Castilho e Thayná Marins

using namespace std;

//structs novas

typedef struct ocorrencia{
    int arquivo; // posicao do arquivo na lista de arquivos processados
    vector<int> linhas; // linhas onde a palavra foi encontrada
}Ocorrencia;

typedef struct palavra{
    string palavra; //palavra em questao
    vector<Ocorrencia> ocorrenciasP; // lista contendo informaçoes da ocorrencia de uma palavra em um arquivo
   // vector<int> ocorrencias; //antigo trabalho
}Palavra;

typedef struct indice{
    vector<string> arquivos; // lista contendo os nomes dos arquivos texto ja processados
    vector<Palavra> listaPalavras; //lista contendo todas as palavras ja encontrada
}Indice;




bool palavraJaexiste(string buscaPalavra, vector<Palavra> &lista){
    lista.begin();
    for (auto &p : lista) {
        if(p.palavra.compare(buscaPalavra) == 0)
            return true;
    }
    return false;
}

int buscarIndex(string buscaPalavra, vector<Palavra> &lista){
    int cont = 0;
    for (auto &p : lista) {
        if(p.palavra.compare(buscaPalavra) == 0)
            return cont;
        cont++;
    }
    return NULL;
}

bool verificaCaracter(string palavra, string c){
    if(palavra.substr(palavra.length()-1).compare(c) == 0)
        return true;
    return false;
}

string verificaFinalPalavra(string palavra){
    string p = palavra.substr(0, palavra.size());

    if(verificaCaracter(p, ".")||verificaCaracter(p, ",")||verificaCaracter(p, "!")||verificaCaracter(p, "?"))
        return p.substr(0, palavra.size()-1);
    return p;
}


// insere uma palavra em ordem alfabética na lista
void InsereOrdem(vector<Palavra> &lista, Palavra palavra){
    int posicao=0; //posição onde a palavra deve entrar

    while (posicao<lista.size() && (palavra.palavra > lista[posicao].palavra) ) //se não chegou na posisção correta, continua
        posicao++;

    lista.insert(lista.begin()+posicao,palavra);//insere na posição correta
}

void inserePalavraOrdem(string p, int linha, vector<Palavra> &lista){
    Palavra palavra1;
    palavra1.palavra = p;
    palavra1.ocorrencias.push_back(linha);
    InsereOrdem(lista, palavra1);
}

void lerArquivo(string nomeArquivo, vector<Palavra> &lista){
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

void escreverIndice(vector<Palavra> &lista){
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

void  buscaPalavra(string palavraB, vector<Palavra> &lista){
    lista.begin();
    int avalia = 0;
    for (auto &p : lista) {
        if(p.palavra.compare(palavraB) == 0){
            cout << "Palavra encontrada :)!"<< endl;
            cout << "A palava apareceu " << p.ocorrencias.size() << " vez(es) no arquivo original." << " Linha(s): ";
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

void utilizarIndice(string nomeA) {

    vector<Palavra> lista;
    int totalp, control=0;

    FILE *aBin;
    aBin = fopen(nomeA.c_str(), "rb");
    fseek(aBin, 0, SEEK_CUR);
    fread(&totalp, sizeof(int),1, aBin);
    cout << "Quantidade total de palavras: " << totalp << endl;

    do{
        Palavra p;

        int c;
        int oc;

        fread(&c, sizeof(int),1, aBin); //caracteres da palavra

        char paux[c];
        fread(paux, c ,1, aBin);
        p.palavra.assign(paux);
        fread(&oc, sizeof(int),1, aBin); //ocorrencias
        p.ocorrencias.resize(oc);
        fread(p.ocorrencias.data(), sizeof(int), oc, aBin);
        //cout << "Qtde. caracteres: " << c << " | Palavra: " << p.palavra << " | Num de ocorrencias: "<< p.ocorrencias.size() <<" | Linhas:"; // impressao com qtde de caracteres do arq binario
        cout << "Palavra: " << p.palavra << " | Num de ocorrencias: "<< p.ocorrencias.size() <<" | Linha(s):";
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
///Trabalho parte 2

/// case 2

void escreveOcorrencia(fstream &arq, Ocorrencia ocorrencia){
    ///escreve o numero do arquivo binario (occorencia int arquivo)
    /// escreve as linhas onde a palavra apareceu neste arquivo


}

void escrevePalavra(fstream &arq, Palavra palavra){
    ///escreve no arq binario as letras da palavra
    ///para cada ocorrecencia
        escreveOcorrencia(arq, palavra.ocorrenciasP[i]);


}
void escreveNomeArquivo(fstream &arq, string nomeArquivo){
   ///escreve a qtde de letras do nome
    ///escreve o nome no arq binario
}
void salvarIndice(Indice ind){
    ///abrir arq binario
    ///escreve a qtde de arquivos
    ///para cada indice de ind.arquivo
        escreveNomeArquivo(arq, ind.arquivos[t]);
     /// escreve a qtde de palavras
    ///para cada palavra de ind.palavras
        escrevePalavra(arquivo, ind.listaPalavras[i]);
    ///fechar arq
}

///case 3

lerNomeArquivo(fstream &arq, Indice ind){

}

lerPalavra(fstream &arq, Indice ind){

}
void lerIndice(Indice &ind) {
    ///liberarIndice(ind); //pode ser ind.clear
    /// abrir o arq binario
    /// ler qtd de arquivos do indice
    /// para cada arquivo
    lerNomeArquivo(arq, ind);
    /// ler a qtde de palavras do indice
    ///para cada palavra
    lerPalavra(arq, ind);
    ///fechar arq
}

int main() {
    vector<Palavra> lista;
    Indice ind;
    int resp = 0;
    string nameArquivo;
    while(resp != -1){
        cout << endl <<"<1>- Criar indice para um arquivo texto" << endl << "<2>- Salvar indice atual"<< endl  << "<3>- Utilizar um indice existente para realizar buscas por palavras"<< endl  <<"<4>- Mostrar as informacoes de um Indice"<< endl <<"<5>- Encerrar"<<endl
             << "<3>- Encerrar o programa" << endl;
        cout << "Resp:";
        cin >> resp;
        string nomeA;

        switch(resp){
            case 1:
                cout << "Nome do arquivo texto:" <<endl;
                cin >> nameArquivo;
                lerArquivo(nameArquivo, lista);
                break;
            case 2:
                salvarIndice(ind);
                cout << "Indice Salvo!" << endl;
                break;
            case 3:
                cin>>nomeA;
                lerIndice(ind);
                break;
            case 4:
                break;
            case 5:
                cout <<"Voce saiu do programa!" << endl;
                resp = -1;
                break;
            default:
                cout << "Opcao digitada nao reconhecida. Tente novamente"<< endl;
        }

    }

    return 0;
}