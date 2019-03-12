#include "util.h"
#include <iostream>

using namespace std;

int main()
{
    char* name11 = "nome-telefone-11.txt";
    char* name100 = "nome-telefone-100.txt";
    char* name1M = "nome-telefone-1M.txt";
    int mem_1MB = 1*1000*1000;
    int mem_500B = 500;
    int mem_60B = 60;


    ///CreateTestFile(name1M, 1000000); ///Cria arquivo de teste no formato exigido.

    if(ExternalOrder(mem_1MB, name1M))
        cout << endl <<  "--> Ordenacao externa concluida com SUCESSO!" << endl;

    return 0;
}
