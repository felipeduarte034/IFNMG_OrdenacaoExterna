#include "util.h"
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

void TrocaStr(vector<string> &v, int a, int b)
{
    string memAux;
    memAux = v[a];
    v[a] = v[b];
    v[b] = memAux;
}

void MaxHeapfyStr(vector<string> &v, int pos, int n)
{
    int maior=2*pos+1, right=maior+1;

    if(maior<n)
    {
        if(right<n && (strcmp(v[maior].c_str(), v[right].c_str()) < 0))
            maior = right;
        if(strcmp(v[maior].c_str(), v[pos].c_str()) > 0)
        {
            TrocaStr(v,maior,pos);
            MaxHeapfyStr(v,maior,n);
        }
    }
}

void BuildMaxHeapStr(vector<string> &v, int n)
{
    for(int i=n/2-1; i>=0; i--)
    {
        MaxHeapfyStr(v,i,n);
    }
}

void HeapSortStr(vector<string> &v, int n)
{
    BuildMaxHeapStr(v,n);
    for(int i=n-1; i>0; i--)
    {
        for(int i=n-1; i>0; i--)
        {
            TrocaStr(v,0,i);
            MaxHeapfyStr(v,0,--n);
        }
    }
}

void StringHeapSort(vector<string> &registers)
{
    //cout << "HEAP BEGIN" << endl;
    /*
    for(int i = 0; i < registers.size(); i++)
    {
        cout << registers[i] << endl;
    }*/

    HeapSortStr(registers, registers.size());
    //cout << "HEAP END" << endl;
}
