#include "rabinkarp.h"

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