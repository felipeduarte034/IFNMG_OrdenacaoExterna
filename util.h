#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

bool ExternalOrder(float mem_max, char* name_file);
void StringHeapSort(vector <string> &registers);
bool IntercalarFiles(char* name_file1, char* name_file2, char* name_fileout, int quantRegByBlock,int tamFileExt,bool* isSort);
bool CreateTestFile(char* name_file, int quant_reg);

#endif // UTIL_H_INCLUDED
