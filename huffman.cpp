#include "huffman.h"

Node::Node(char c,int f){
    ch=c;
    freq=f;
    left=NULL;
    right=NULL;
}

struct cmp{
    bool operator()(Node* a,Node* b){
        return a->freq > b->freq;
    }
};

void mergeArr(vector<pair<char,int>> &a,int l,int m,int r){
    vector<pair<char,int>> temp;
    int i=l,j=m+1;

    while(i<=m && j<=r){
        if(a[i].second < a[j].second)
            temp.push_back(a[i++]);
        else
            temp.push_back(a[j++]);
    }

    while(i<=m) temp.push_back(a[i++]);
    while(j<=r) temp.push_back(a[j++]);

    for(int k=l;k<=r;k++)
        a[k]=temp[k-l];
}

void mergeSort(vector<pair<char,int>> &a,int l,int r){
    if(l>=r) return;
    int mid=(l+r)/2;
    mergeSort(a,l,mid);
    mergeSort(a,mid+1,r);
    mergeArr(a,l,mid,r);
}

Node* buildTree(vector<pair<char,int>> freqList){
    priority_queue<Node*,vector<Node*>,cmp> pq;

    for(auto x:freqList)
        pq.push(new Node(x.first,x.second));

    while(pq.size()>1){
        Node* left=pq.top(); pq.pop();
        Node* right=pq.top(); pq.pop();

        Node* parent=new Node('#',left->freq+right->freq);
        parent->left=left;
        parent->right=right;

        pq.push(parent);
    }

    return pq.top();
}

void generate(Node* root,string code,unordered_map<char,string> &mp){
    if(!root) return;

    if(!root->left && !root->right)
        mp[root->ch]=code;

    generate(root->left,code+"0",mp);
    generate(root->right,code+"1",mp);
}

string compressFile(string inputFile,Node* &root){
    ifstream in(inputFile);

    string text="",line;
    while(getline(in,line))
        text += line + "\n";
    in.close();

    unordered_map<char,int> freq;
    for(char c:text)
        freq[c]++;

    vector<pair<char,int>> freqList(freq.begin(), freq.end());

    mergeSort(freqList,0,freqList.size()-1);

    root=buildTree(freqList);

    unordered_map<char,string> code;
    generate(root,"",code);

    string encoded="";
    for(char c:text)
        encoded+=code[c];

    return encoded;
}

string decompress(Node* root,string encoded){
    Node* curr=root;
    string decoded="";

    for(char bit:encoded){
        if(bit=='0') curr=curr->left;
        else curr=curr->right;

        if(!curr->left && !curr->right){
            decoded+=curr->ch;
            curr=root;
        }
    }

    return decoded;
}