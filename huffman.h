#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <bits/stdc++.h>
using namespace std;

struct Node{
    char ch;
    int freq;
    Node *left,*right;

    Node(char c,int f);
};

Node* buildTree(vector<pair<char,int>> freqList);
void generate(Node* root,string code,unordered_map<char,string> &mp);

string compressFile(string inputFile,Node* &root);
string decompress(Node* root,string encoded);

#endif