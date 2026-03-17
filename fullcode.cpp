#include <bits/stdc++.h>
using namespace std;

struct Node{
    char ch;
    int freq;
    Node *left,*right;

    Node(char c,int f){
        ch=c;
        freq=f;
        left=NULL;
        right=NULL;
    }
};

struct cmp{
    bool operator()(Node* a,Node* b){
        return a->freq > b->freq;
    }
};

// ---------------- MERGE SORT ----------------

void mergeArr(vector<pair<char,int>> &a,int l,int m,int r){

    vector<pair<char,int>> temp;

    int i=l,j=m+1;

    while(i<=m && j<=r){

        if(a[i].second < a[j].second){
            temp.push_back(a[i]);
            i++;
        }
        else{
            temp.push_back(a[j]);
            j++;
        }

    }

    while(i<=m){
        temp.push_back(a[i]);
        i++;
    }

    while(j<=r){
        temp.push_back(a[j]);
        j++;
    }

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

// ---------------- BUILD HUFFMAN TREE ----------------

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

// ---------------- GENERATE HUFFMAN CODES ----------------

void generate(Node* root,string code,unordered_map<char,string> &mp){

    if(root==NULL) return;

    if(!root->left && !root->right)
        mp[root->ch]=code;

    generate(root->left,code+"0",mp);
    generate(root->right,code+"1",mp);
}

// ---------------- COMPRESS FILE ----------------

string compressFile(string inputFile,Node* &root){

    ifstream in(inputFile);

    string text="",line;

    while(getline(in,line))
        text += line + "\n";

    in.close();

    unordered_map<char,int> freq;

    for(char c:text)
        freq[c]++;

    vector<pair<char,int>> freqList;

    for(auto x:freq)
        freqList.push_back(x);

    mergeSort(freqList,0,freqList.size()-1);

    root=buildTree(freqList);

    unordered_map<char,string> code;

    generate(root,"",code);

    string encoded="";

    for(char c:text)
        encoded+=code[c];

    return encoded;
}

// ---------------- DECOMPRESS ----------------

string decompress(Node* root,string encoded){

    Node* curr=root;

    string decoded="";

    for(char bit:encoded){

        if(bit=='0')
            curr=curr->left;
        else
            curr=curr->right;

        if(!curr->left && !curr->right){
            decoded+=curr->ch;
            curr=root;
        }

    }

    return decoded;
}

// ---------------- RABIN KARP ----------------

void rabinKarp(string text,string pat){

    int n=text.size();
    int m=pat.size();

    int d=256;
    int q=101;

    int p=0,t=0,h=1;

    for(int i=0;i<m-1;i++)
        h=(h*d)%q;

    for(int i=0;i<m;i++){
        p=(d*p+pat[i])%q;
        t=(d*t+text[i])%q;
    }

    for(int i=0;i<=n-m;i++){

        if(p==t){

            bool flag=true;

            for(int j=0;j<m;j++){
                if(text[i+j]!=pat[j]){
                    flag=false;
                    break;
                }
            }

            if(flag)
                cout<<"Pattern found at index "<<i<<endl;
        }

        if(i<n-m){
            t=(d*(t-text[i]*h)+text[i+m])%q;
            if(t<0) t+=q;
        }

    }
}
long getFileSize(string filename)
{
    ifstream file(filename, ios::binary | ios::ate);
    return file.tellg();
}
// ---------------- MAIN ----------------

int main(){

    string inputFile;

    cout<<"Enter .txt file name: ";
    cin>>inputFile;

    Node* root=NULL;

    string encoded = compressFile(inputFile,root);

    cout<<"\nCompressed Data:\n";
    cout<<encoded<<endl;
    long originalSize = getFileSize(inputFile);

cout << "\nCompressed Data:\n";
cout << encoded << endl;

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