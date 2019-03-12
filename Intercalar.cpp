#include "util.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>

using namespace std;

bool IntercalarFiles(char* name_file1, char* name_file2, char* name_fileout,int quantRegByBlock,int tamFileExt,bool* isSort)
{
    ifstream file1;
    ifstream file2;
    ifstream fileOut;
    ofstream fileOutW;
    ofstream file1W;
    ofstream file2W;

    vector <string> filesEnd;

    char* bufferF1 = new char [30];
    char* bufferF2 = new char [30];

    char* backF1 = new char [30];
    char* backF2 = new char [30];

    bool isEndFileF1 = false;
    bool isEndFileF2 = false;
    bool isEndBlockF1 = false;
    bool isEndBlockF2 = false;

    ///int counterRegF1=0;
    ///int counterRegF2=0;

    cout << " f1: " << name_file1 << " f2: " << name_file2 << " fout: " << name_fileout << endl;

    fileOutW.open(name_fileout, ios::out | ios::app | ios::binary);
    if(!fileOutW.is_open()) /// Verifica se o arquivo externo existe
    {
        cout <<  "ALERTA: Nao foi possivel abrir/criar o arquivo externo! nome: " << name_fileout << endl;
        return false;
    }
    file1.open(name_file1);
    file2.open(name_file2);
    if(file1.is_open() && file2.is_open())
    {
        file1.read(bufferF1,30); ///ler o primeiro valor do bloco
        file2.read(bufferF2,30); ///ler o primeiro valor do bloco
        ///counterRegF1++;
        ///counterRegF2++;
        ///strcpy(valueF1,bufferF1);
        ///strcpy(valueF2,bufferF2);

        int counter=1;
        //while( !(isEndBlockF1 && isEndBlockF2) )
        while( true )
        {
            /*
            cout << "  indice: " << counter++ << endl;
            cout << "bufferF1: " << bufferF1;
            cout << "bufferF2: " << bufferF2;
            */

            if(strcmp(bufferF1,bufferF2)<=0 && !isEndBlockF1)
            {
                fileOutW << bufferF1;
                ///cout << "out: " << bufferF1;
                strcpy(backF1,bufferF1);
                file1.read(bufferF1,30); ///avança para o proximo registro do file1
                ///counterRegF1++;
                if(strcmp(backF1,bufferF1)>0) ///final do bloco do file1
                    isEndBlockF1=true;
                ///if(counterRegF1==quantRegByBlock)
                ///    isEndBlockF1=true;
            }
            else if(strcmp(bufferF1,bufferF2)>0 && !isEndBlockF2)
            {
                fileOutW << bufferF2;
                ///cout << "out: " << bufferF2;
                strcpy(backF2,bufferF2);
                file2.read(bufferF2,30); ///avança para o proximo registro do file2
                ///counterRegF2++;
                if(strcmp(backF2,bufferF2)>0) ///final do bloco do file2
                    isEndBlockF2=true;
                ///if(counterRegF2==quantRegByBlock)
                ///    isEndBlockF2=true;
            }
            else if(isEndBlockF1 && !isEndBlockF2)
            {
                fileOutW << bufferF2;
                ///cout << "out: " << bufferF2;
                strcpy(backF2,bufferF2);
                file2.read(bufferF2,30); ///avança para o proximo registro do file2
                ///counterRegF2++;
                if(strcmp(backF2,bufferF2)>0) ///final do bloco do file2
                    isEndBlockF2=true;
                ///if(counterRegF2==quantRegByBlock)
                ///    isEndBlockF2=true;
            }
            else if(isEndBlockF2 && !isEndBlockF1)
            {
                fileOutW << bufferF1;
                ///cout << "out: " << bufferF1;
                strcpy(backF1,bufferF1);
                file1.read(bufferF1,30); ///avança para o proximo registro do file1
                ///counterRegF1++;
                if(strcmp(backF1,bufferF1)>0) ///final do bloco do file1
                    isEndBlockF1=true;
                ///if(counterRegF1==quantRegByBlock)
                ///    isEndBlockF1=true;
            }

            if(strcmp(backF1,bufferF1)==0) ///final do arquivo 1
            {
                isEndFileF1=true;
                filesEnd.push_back("file1");
            }
            if(strcmp(backF2,bufferF2)==0) ///final do arquivo 2
            {
                isEndFileF2=true;
                filesEnd.push_back("file2");
            }

            if(isEndFileF1)
            {
                isEndBlockF1=true;
            }
            if(isEndFileF2)
            {
                isEndBlockF2=true;
            }

            ///cout << " isEndBlockF1: " << isEndBlockF1 << "  isEndBlockF2: " << isEndBlockF2 << endl;
            if(isEndBlockF1 && isEndBlockF2 && !(isEndFileF1 || isEndFileF2))
            {
                isEndBlockF1 = isEndBlockF2 = false;
                ///counterRegF1 = counterRegF2 = 0;
            }

            if( (isEndFileF1 && isEndBlockF2) || (isEndFileF2 && isEndBlockF1))
            {
                fileOutW.close();
                int tamFileOut=0;
                fileOut.open(name_fileout);
                fileOut.seekg(0,ios::end);
                tamFileOut = fileOut.tellg(); ///recupera o tamanho do arquivo de saida
                fileOut.seekg(0,ios::beg);
                fileOut.close();
                cout << " tamFileOut: " << tamFileOut << "  tamFileExt: " << tamFileExt << endl << endl;

                if(tamFileOut >= tamFileExt) ///arquivo ordenado
                {
                    file1.close();
                    file2.close();
                    ///fileOutW.close();
                    remove(name_file1);
                    remove(name_file2);
                    cout << "  ARQUIVO ORDENADO: " << name_fileout << "  size: " << tamFileOut << "/" << tamFileExt << " Bytes" << endl;
                    *isSort = true;
                    return true;
                }

                if(filesEnd[0]=="file1")
                {
                    file1.close();
                    file1W.open(name_file1, ios::trunc);

                    file1W << bufferF2; ///pega o registro do loop anterior
                    while(file2.read(bufferF2,30))
                    {
                        file1W << bufferF2;
                    }
                    file1W.close();

                    file2.close();
                    file2W.open(name_file2, ios::trunc);
                    file2W.close();
                }
                else
                {
                    file2.close();
                    file2W.open(name_file2, ios::trunc);

                    file2W << bufferF1; ///pega o registro do loop anterior
                    while(file1.read(bufferF1,30))
                    {
                        file2W << bufferF1;
                    }
                    file2W.close();

                    file1.close();
                    file1W.open(name_file1, ios::trunc);
                    file1W.close();
                }
                break;
            }
        }
    }
    else
    {
        cout <<  "IntercalarFiles(): Não foi possivel abrir/criar os arquivos! nome: " << name_file1 << " e " << name_file2 << endl;
        return false;
    }
    fileOutW.close();
    file1.close();
    file2.close();

    ///cout << " filesEnd[0]: " << filesEnd[0] << "  size: " << filesEnd.size() << endl;
    /*
    if(filesEnd[0]=="file1")
    {
        file1W.open(name_file1, ios::trunc);
        file1W.close();
    }
    else
    {
        file2W.open(name_file2, ios::trunc);
        file2W.close();
    }
    */

    return true;
}
