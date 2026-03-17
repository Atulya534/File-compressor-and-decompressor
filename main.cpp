#include "huffman.h"
#include "rabinkarp.h"
#include "utils.h"

int main(){

    string inputFile;
    cout<<"Enter .txt file name: ";
    cin>>inputFile;

    Node* root=NULL;

    string encoded = compressFile(inputFile,root);

    cout<<"\nCompressed Data:\n"<<encoded<<endl;

    long originalSize = getFileSize(inputFile);
    long compressedSize = encoded.size() / 8;

    cout << "\nOriginal File Size: " << originalSize << " bytes\n";
    cout << "Compressed Size (approx): " << compressedSize << " bytes\n";

    double ratio = ((double)compressedSize / originalSize) * 100;
    cout << "Compression Ratio: " << ratio << "%\n";

    string decoded = decompress(root,encoded);

    ofstream out("decompressed.txt");
    out<<decoded;
    out.close();

    cout<<"\nFile decompressed and saved as decompressed.txt\n";

    string pattern;
    cout<<"\nEnter word to search using Rabin Karp: ";
    cin>>pattern;

    rabinKarp(decoded,pattern);

    return 0;
}