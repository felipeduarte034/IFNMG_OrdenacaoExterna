#include "util.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

bool DistribuicaoDesbalanceada(vector<string> &v, char* name_file1, char* name_file2, int quantBlocksF1, int* cbt1, int* cbt2)
{
    ofstream fileTemp1W;
    ofstream fileTemp2W;

    if(*cbt1 < quantBlocksF1)
    {
        fileTemp1W.open(name_file1, ios::out | ios::app | ios::binary);
        if(fileTemp1W.is_open())
        {
            ///*counterBF1 += 1;
            for(int i=0; i < v.size(); i++)
                fileTemp1W << v[i] << "\n";
            *cbt1 += 1;
        }
        else
        {
            cout <<  "DistribuicaoBalanceada(): Não foi possivel abrir/criar o arquivo! nome: temp1.txt" << endl;
            return false;
        }
        fileTemp1W.close();
    }
    else
    {
        fileTemp2W.open(name_file2, ios::out | ios::app | ios::binary);
        if(fileTemp2W.is_open())
        {
            for(int i=0; i < v.size(); i++)
                fileTemp2W << v[i] << "\n";
            *cbt2 += 1;
        }
        else
        {
            cout <<  "DistribuicaoBalanceada(): Não foi possivel abrir/criar o arquivo! nome: temp2.txt" << endl;
            return false;
        }
        fileTemp2W.close();
    }
}

bool DistribuicaoBalanceada(vector<string> &v, char* name_file1, char* name_file2, bool* isControle, int* cbt1, int* cbt2)
{
    ofstream fileTemp1W;
    ofstream fileTemp2W;

    ///Distribuição BEGIN
    if(*isControle) ///Distribui balançeada
    {
        *isControle = !(*isControle);
        fileTemp1W.open(name_file1, ios::out | ios::app | ios::binary);
        if(fileTemp1W.is_open())
        {
            for(int i=0; i < v.size(); i++)
                fileTemp1W << v[i] << endl;
            *cbt1 += 1;
        }
        else
        {
            cout <<  "DistribuicaoBalanceada(): Não foi possivel abrir/criar o arquivo! nome: temp1.txt" << endl;
            return false;
        }
        fileTemp1W.close();
    }
    else
    {
        *isControle = !(*isControle);
        fileTemp2W.open(name_file2, ios::out | ios::app | ios::binary);
        if(fileTemp2W.is_open())
        {
            for(int i=0; i < v.size(); i++)
                fileTemp2W << v[i] << endl;
            *cbt2 += 1;
        }
        else
        {
            cout <<  "DistribuicaoBalanceada(): Não foi possivel abrir/criar o arquivo! nome: temp2.txt" << endl;
            return false;
        }
        fileTemp2W.close();
    }
    ///Distribuição END
    return true;
}

int GetSizeFile(char * name_file)
{
    int tam;
    ifstream file;
    file.open(name_file);
    if(file.is_open())
    {
        file.seekg(0,ios::end);
        tam = file.tellg();
        file.seekg(0,ios::beg);
    }
    else
        return -1;
    file.close();
    return tam;
}

///função para fazer a ordenação externa
bool ExternalOrder(float mem_max, char* name_file)
{
    ifstream fileExt;
    ifstream fileTemp1;
    ifstream fileTemp2;
    ifstream fileSort;
    ofstream fileTemp1W;
    ofstream fileTemp2W;
    ofstream fileSortW;

    ///const char* new_path = "./" + name_file + ".ordenado";

    float tamRegistro = 30; /// (29B + 1B) --> 1Byte por caracter + 1Byte da quebra de linha (\n)
    float tamFileExt;
    float quantBlocks;
    float tamBlock;
    float quantRegByBlock;
    float quantRegInFile;
    int counterItens=0;
    int totalItensLidos=0;
    string line;
    vector <string> registros;
    int currentBlock=1;
    float quantRegInLastBlock=0;
    int counterBlocksTemp1=0;
    int counterBlocksTemp2=0;
    int* cbt1 = &counterBlocksTemp1;
    int* cbt2 = &counterBlocksTemp2;
    bool isControle=true; ///controle da distribuição balançeada
    bool* isPControle = &isControle;
    float tamFT1=0;
    float tamFT2=0;
    float tamFS=0;
    int quantBlocksF1;
    bool isSort=false;
    bool *pIsSort=&isSort;
    ///int counterBF1;
    ///int* pQuantBlocksF1 = &quantBlocksF1;
    ///int* pCounterBF1 = &counterBF1;

    fileExt.open(name_file);
    if(!fileExt.is_open()) /// Verifica se o arquivo externo existe
    {
        cout <<  "ALERTA: Nao foi possivel ler o arquivo externo! nome: " << name_file << endl;
        return false;
    }

    ///se os arquivos já existirem, são limpos.
    fileSortW.open("sort.txt", ios::trunc);
    fileSortW.close();
    fileTemp1W.open("temp1.txt", ios::trunc);
    fileTemp1W.close();
    fileTemp2W.open("temp2.txt", ios::trunc);
    fileTemp2W.close();

    ///calculos uteis
    fileExt.seekg(0,ios::end);
    tamFileExt = fileExt.tellg();
    fileExt.seekg(0,ios::beg);
    tamBlock = mem_max;
    quantRegByBlock = floor(tamBlock/tamRegistro);
    quantRegInFile = tamFileExt/tamRegistro;
    quantBlocks = ceil(quantRegInFile/quantRegByBlock);
    quantRegInLastBlock = quantRegInFile - ((quantBlocks-1) * quantRegByBlock);
    quantBlocksF1 = (quantBlocks/2)+1;

    cout <<  "--> Memoria disponibilizada: " << mem_max << " Bytes - " << mem_max/1000 << " KBytes - " << mem_max/1000/1000 << " MBytes" << endl;
    cout <<  "--> Tamanho dos Blocos: " << tamBlock << " Bytes - "  << tamBlock/1000 << " KBytes - " << tamBlock/1000/1000 << " MBytes" << endl;
    cout <<  "--> Tamanho do arquivo externo: " << tamFileExt << " Bytes - " << tamFileExt/1000 << " KBytes - " << tamFileExt/1000/1000 << " MBytes" << endl;
    cout <<  "--> Tamanho do Registro: " << tamRegistro << " bytes" << endl;
    cout <<  "--> Quantidade de blocos: " << quantBlocks << " blocos." << endl;
    cout <<  "--> Quantidade de registros no arquivo externo: " << quantRegInFile << " - " << quantRegInFile/1000 << "K - " << quantRegInFile/1000/1000 << "M" << endl;
    cout <<  "--> Quantidade de Registros por Bloco: " << quantRegByBlock << endl;
    ///cout <<  "--> quantBlocksF1: " << quantBlocksF1 << endl;
    cout <<  "--> Quantidade de registros no ultimo bloco: " << quantRegInLastBlock << " - " << quantRegInLastBlock/1000 << "K - " << quantRegInLastBlock/1000/1000 << "M" << endl << endl;
    ///return false;

    while(getline(fileExt, line))
    {
        if(counterItens < quantRegByBlock)
        {
            registros.push_back(line); ///armazena o registro atual no bloco
            counterItens++;
            totalItensLidos++;

            if(quantRegInLastBlock<quantRegByBlock && currentBlock==quantBlocks && counterItens==quantRegInLastBlock) ///verifica se o ultimo bloco tem menos registros os demais.
            {
                StringHeapSort(registros);
                ///DistribuicaoBalanceada(registros,"temp1.txt","temp2.txt",isPControle,cbt1,cbt2);
                DistribuicaoDesbalanceada(registros,"temp1.txt","temp2.txt",quantBlocksF1,cbt1,cbt2);
                cout << "===== BLOCO: " << currentBlock << "/" << quantBlocks << " OK.  counterItens: " << counterItens << "  (ultimo bloco)." << endl;
                currentBlock++;
                registros.clear();
                counterItens=0;
            }
        }
        else
        {
            StringHeapSort(registros);
            ///DistribuicaoBalanceada(registros,"temp1.txt","temp2.txt",isPControle,cbt1,cbt2);
            DistribuicaoDesbalanceada(registros,"temp1.txt","temp2.txt",quantBlocksF1,cbt1,cbt2);
            cout << "===== BLOCO: " << currentBlock << "/" << quantBlocks << " OK.  counterItens: " << counterItens << endl;
            currentBlock++;
            registros.clear();
            counterItens=0;

            registros.push_back(line); ///armazena o registro atual no bloco
            counterItens++;
            totalItensLidos++;

            if(quantRegInLastBlock<quantRegByBlock && currentBlock==quantBlocks && counterItens==quantRegInLastBlock) ///verifica se o ultimo bloco tem menos registros os demais.
            {
                StringHeapSort(registros);
                ///DistribuicaoBalanceada(registros,"temp1.txt","temp2.txt",isPControle,cbt1,cbt2);
                DistribuicaoDesbalanceada(registros,"temp1.txt","temp2.txt",quantBlocksF1,cbt1,cbt2);
                cout << "===== BLOCO: " << currentBlock << "/" << quantBlocks << " OK.  counterItens: " << counterItens << "  (ultimo bloco)." << endl;
                currentBlock++;
                registros.clear();
                counterItens=0;
            }
        }
    }
    fileExt.close();

    cout << endl;
    cout << " totalItensLidos: " << totalItensLidos << endl;
    cout << " counterBlocksTemp1: " << counterBlocksTemp1 << endl;
    cout << " counterBlocksTemp2: " << counterBlocksTemp2 << endl;


    cout << endl << endl;
    cout << "===== INTERCALACAO: =====" << endl;
    int countInter=1;
    while(!(*pIsSort))
    {
        tamFS = GetSizeFile("sort.txt");
        tamFT1 = GetSizeFile("temp1.txt");
        tamFT2 = GetSizeFile("temp2.txt");

        cout << "===== countInter: " << countInter++ << " - ";
        ///*
        cout << "sort.txt: " << tamFS << " - ";
        cout << "temp1.txt: " << tamFT1 << " - ";
        cout << "temp2.txt: " << tamFT2 << endl;
        //*/

        if(tamFS == 0)
            IntercalarFiles("temp1.txt", "temp2.txt", "sort.txt",quantRegByBlock,tamFileExt,pIsSort);
        else if(tamFT1 == 0)
            IntercalarFiles("sort.txt", "temp2.txt", "temp1.txt",quantRegByBlock,tamFileExt,pIsSort);
        else if(tamFT2 == 0)
            IntercalarFiles("sort.txt", "temp1.txt", "temp2.txt",quantRegByBlock,tamFileExt,pIsSort);
        else
        {
            cout <<  "ALERTA: nenhum arquivo livre, para fazer a intercalacao." << endl;
            return false;
        }
        if(countInter==1)
            break;
    }

    return true;
}
