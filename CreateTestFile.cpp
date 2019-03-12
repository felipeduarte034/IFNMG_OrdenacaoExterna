#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

string RandomText()
{
    string text;
    int numLetras = 3+(rand()%17);
    //return numLetras;
    for(int i=0; i<numLetras; i++)
    {
        text += 'a'+(char)(rand()%26);
    }
    int numSpaces = 19-numLetras;
    for(int i=0; i<numSpaces; i++)
    {
        text +=" ";
    }
    //cout << text << endl;
    return text;
}

string RandomNumber()
{
    string txt;
    for(int i=0; i<4; i++)
    {
        txt += 48+(char)(rand()%10);
    }
    return txt;
}

bool CreateTestFile(char* name_file, int quant_reg)
{
    string nome;
    string telefone;
    ofstream fileW;
    fileW.open(name_file, ios::out | ios::app | ios::binary);
    if(!fileW.is_open())
    {
        cout <<  "ALERT: Não foi possivel abrir/criar o arquivo! nome: " << name_file << endl;
        return false;
    }
    for(int i=0; i<quant_reg; i++)
    {
        string nome = RandomText();
        string telefone = RandomNumber() + "-" + RandomNumber();
        string item = nome + ":" + telefone;
        fileW << item << endl;
    }
    fileW.close();

    return true;
}
