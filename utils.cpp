#include "utils.h"

long getFileSize(string filename){
    ifstream file(filename, ios::binary | ios::ate);
    return file.tellg();
}