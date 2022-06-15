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


//codigos do trabalho parte 1

bool palavraJaexiste(string buscaPalavra, Indice &lista){
    for (auto &p : lista.listaPalavras) {
        if(p.palavra.compare(buscaPalavra) == 0)
            return true;
    }
    return false;
}

int buscarIndex(string buscaPalavra, Indice &lista){
    int cont = 0;
    for (auto &p : lista.listaPalavras) {
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

int verificaSeArquivoFoiProcessado(string nomeArquivo, Indice lista){
    int cont = 0;
    for(string p : lista.arquivos){
        if(p.compare(nomeArquivo) == 0)
            return cont;
        cont++;
    }
    return 0;
}

int verificaSePalavraFoiProcessado(string nomeArquivo, Indice lista, int index){
    int cont = 0;
    int indexArquivo = verificaSeArquivoFoiProcessado(nomeArquivo, lista);
    for(Ocorrencia o : lista.listaPalavras[index].ocorrenciasP){
        if(o.arquivo == indexArquivo)
            return cont;
        cont++;
    }
    return 0;
}

void adicionaPalavra(string p, int linha, Indice &lista, string nomeArquivo, int index){
    int indexPalavra = verificaSePalavraFoiProcessado(nomeArquivo, lista, index);
    if(indexPalavra!=0){
        lista.listaPalavras[index].ocorrenciasP[indexPalavra].linhas.push_back(linha);
    }
    else {
        Ocorrencia ocorrencia1;
        ocorrencia1.arquivo = verificaSeArquivoFoiProcessado(nomeArquivo, lista);
        ocorrencia1.linhas.push_back(linha);
        lista.listaPalavras[index].ocorrenciasP.push_back(ocorrencia1);
    }

}

// insere uma palavra em ordem alfabética na lista

void inserePalavraOrdem(string p, int linha, Indice &lista, string nomeArquivo){
    Palavra palavra1;
    Ocorrencia ocorrencia1;
    ocorrencia1.arquivo = verificaSeArquivoFoiProcessado(nomeArquivo, lista);
    ocorrencia1.linhas.push_back(linha);
    palavra1.palavra = p;
    palavra1.ocorrenciasP.push_back(ocorrencia1);


    int posicao=0; //posição onde a palavra deve entrar

    while (posicao<lista.listaPalavras.size() && (palavra1.palavra > lista.listaPalavras[posicao].palavra) ) //se não chegou na posisção correta, continua
        posicao++;

    lista.listaPalavras.insert(lista.listaPalavras.begin()+posicao,palavra1);//insere na posição correta
}

void lerArquivo(string nomeArquivo, Indice &lista) {
    if (verificaSeArquivoFoiProcessado(nomeArquivo, lista)!=0) {
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
                    adicionaPalavra(palavraTexto, cont, lista, nomeArquivo, index);
                } else {
                    inserePalavraOrdem(palavraTexto, cont, lista, nomeArquivo);
                }
            }
            cont++;
        }
        arq.close();
    }
}

/*
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
*/
/*
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
*/
/*
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

 */

///Trabalho parte 2

/// case 2

void escreveOcorrencia(FILE *arq, Ocorrencia ocorrencia){
    ///escreve o numero do arquivo binario (occorencia int arquivo)
    fwrite(&ocorrencia.arquivo, sizeof(int), 1, arq);
    ///qtde de vezes que a palavra apareceu no arquivo
    int oc = ocorrencia.linhas.size();
    fwrite(&oc, sizeof(int), 1, arq);
    /// escreve as linhas onde a palavra apareceu neste arquivo
    fwrite(ocorrencia.linhas.data(), sizeof(int), ocorrencia.linhas.size(), arq);

}

void escrevePalavra(FILE *arq, Palavra palavra){

    ///escreve qtde de letras da palavra + 1;
    int caracteres = palavra.palavra.size()+1;
    fwrite(&caracteres, sizeof(int), 1, arq);

    ///escreve no arq binario as letras da palavra + /0
    string palavraP = palavra.palavra + '\0';
    fwrite(palavraP.c_str(), caracteres, 1, arq);

    /// ocorrencias total
    int octotal = palavra.ocorrenciasP.size();
    fwrite(&octotal, sizeof(int), 1, arq);


    ///para cada ocorrecencia
    for(int i = 0; i <= palavra.ocorrenciasP.size(); i++){
        escreveOcorrencia(arq, palavra.ocorrenciasP[i]);
    }



}
void escreveNomeArquivo(FILE *arq, string nomeArquivo){
   ///escreve a qtde de letras do nome
    int caracteres = nomeArquivo.size()+1;
    fwrite(&caracteres, sizeof(int), 1, arq);
    ///escreve o nome no arq binario
    string palavraP;
    palavraP = nomeArquivo + '\0';
    fwrite(palavraP.c_str(), caracteres, 1, arq);



}
void salvarIndice(Indice &ind){
    ///abrir arq binario
    FILE *arq;
    arq = fopen("indice.dat", "wb");
    ///escreve a qtde de arquivos texto processados
    int size = ind.arquivos.size();
    fseek(arq, 0, SEEK_END);
    fwrite(&size, sizeof(int), 1, arq);
    ///para cada arquivo de ind.arquivo
    for(int i = 0; i <= ind.arquivos.size(); i++){
        escreveNomeArquivo(arq, ind.arquivos[i]);

    }
     /// escreve a qtde de palavras
     int qtdePalavras = ind.listaPalavras.size();
     fwrite(&qtdePalavras, sizeof(int), 1, arq);

    ///para cada palavra de ind.palavras
    for(int j = 0; j <= ind.listaPalavras.size(); j++){
        escrevePalavra(arq, ind.listaPalavras[j]);
    }

    ///fechar arq
    fclose(arq);
}

///case 3


void lerOcorrencia(FILE *arq, Ocorrencia &ocorrencia){
    ///ler o numero do arquivo binario (ocorrencia int arquivo)
    fread(&ocorrencia.arquivo, sizeof(int), 1, arq);

    /// ocorrencia no arquivo
    int oc;
    fread(&oc, sizeof(int), 1, arq);
    ocorrencia.linhas.resize(oc);

   /// ler as linhas onde a palavra apareceu neste arquivo
   fread(ocorrencia.linhas.data(), sizeof(int), oc, arq);


}

void lerPalavra(FILE *arq, Indice &ind){
    Palavra p;
    ///lendo a qtde de letras da palavra
    int qtdletraPal;
    fread(&qtdletraPal, sizeof(int), 1, arq);

    ///ler no arq binario as letras da palavra
    char letras[qtdletraPal];
    fread(letras, qtdletraPal ,1, arq);

    p.palavra = letras;

    ///lendo a ocorrencia total
    int oct;
    fread(&oct, sizeof(int), 1, arq);
    p.ocorrenciasP.resize(oct);


    ///ler para cada ocorrecencia
    for(int i = 0; i <= oct; i++){
        lerOcorrencia(arq,p.ocorrenciasP[i]);
    }

    ind.listaPalavras.push_back(p);

}

void lerNomeArquivo(FILE *arq, Indice &ind){
    ///ler a qtde de letras do nome
    int letrasqtd;
    fread(&letrasqtd, sizeof(int), 1, arq);

    ///ler o nome no arq binario
    char nome[letrasqtd];
    fread(nome, letrasqtd ,1, arq);

    ind.arquivos.push_back(nome);


}

void lerIndice(Indice &ind) {
    ///liberarIndice(ind);
    ind.arquivos.clear();
    ind.listaPalavras.clear();

    /// abrir o arq binario
    FILE *arq;
    arq = fopen("indice.dat", "rb");

    /// ler qtd de arquivos do indice
    int qtdArq;
    fread(&qtdArq, sizeof(int), 1, arq);

    ind.arquivos.resize(qtdArq);   // p.ocorrencias.resize(oc);
    /// para cada arquivo
    for(int i = 0; i <= qtdArq; i++){
        lerNomeArquivo(arq, ind);
    }

    /// ler a qtde de palavras do indice
    int qtdPal;
    fread(&qtdPal, sizeof(int), 1, arq);
    ind.listaPalavras.resize(qtdPal);

    ///para cada palavra
    for(int j = 0; j <= qtdPal; j++){
        lerPalavra(arq, ind);
    }
    ///fechar arq
    fclose(arq);
}

int main() {
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
                lerArquivo(nameArquivo, ind);
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