#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <fstream>
#include<string>
#include<algorithm>
#include <map>
#include <vector>
#include"File.h"
#include"Ngram.h"
#include"Vocab.h"

using namespace std;

char seg_file[256];
char ztob[256];
char lmodel[256];
int order;
string line;
string problem;
Vocab voc;
Ngram lm(voc,order);

map<string,vector<string>> dict;
void Dealdiction(){
    ifstream mapping;
    mapping.open(ztob);
    while(getline (mapping, line)){//deal with map
        string zhu;
        zhu.assign(line.begin(),line.begin()+2);
       for(int i=1;i<line.size()/3;i++){
        string help;
        help.assign(line.begin()+3*i,line.begin()+3*i+2);
        dict[zhu].push_back(help);
       }
    }
    mapping.close();
}
double probab(const char* word1,const char* word2){//P(word2|word1)
    VocabIndex wid1 = voc.getIndex(word1);
    if(wid1 == Vocab_None) 
        wid1= voc.getIndex(Vocab_Unknown);
    VocabIndex wid2 = voc.getIndex(word2);
    if(wid2 == Vocab_None) 
        wid2= voc.getIndex(Vocab_Unknown);
    VocabIndex context[] = {wid1, Vocab_None};
    return lm.wordProb(wid2, context);

}
int main(int argc, char *argv[]){
	strcpy(seg_file, argv[2]);
	strcpy(ztob, argv[4]);
	strcpy(lmodel, argv[6]);
	order=stoi(argv[8]);
    File lmFile( lmodel, "r" );
    lm.read(lmFile);
    lmFile.close();
    Dealdiction();
    ifstream test_data;
    test_data.open(seg_file);//deal with each line
    while(getline(test_data,line)){//處理每一行
        double viter[15000][15000]={{0.0}};
        int trace[15000][15000]={{0}};
        auto itor = remove_if(line.begin(), line.end(), ::isspace);
        line.erase(itor, line.end());//去除空白
        string word;
        word.assign(line.begin(),line.begin()+2);
        for(int i=0;i<dict[word].size();i++)
            viter[i][0]=probab("<s>",dict[word][i].c_str());//P(第0個字｜<s>)
        for(int i=1;i<line.size()/2;i++){//橫軸
            string prev(word);
            word.assign(line.begin()+2*i,line.begin()+2*i+2);
            for(int j=0;j<dict[word].size();j++){//縱軸
                double max=-1;
                int maxdex=-1;
                for(int k=0;k<dict[prev].size();k++){
                    double now=probab(dict[word][j],dict[prev][k])+viter[k][i-1];//problem
                    if(now>max){
                        max=now;
                        maxdex=k;
                    }
                }
                viter[j][i]=max;
                trace[j][i]=maxdex;
            }
        }
        string prev(word);
        double max=-1;int maxdex=-1;
        for(int i=0;i<dict[prev].size();i++){
            double now=probab(dict[prev][i].c_str(),"</s>");
            if(now>max){
                max=now;
                maxdex=k;
            }
        }
        vector<string> answer;
        int index=maxdex;
        for(int i=line.size()/2-1;i>=0;i--){
            string help;
            help.assign(line.begin()+2*i,line.begin()+2*i+2);
            answer.pushback(dict[help][index]);
            index=trace[index][i];
        }
        cout << "<s> ";
        for(int i=line.size()/2-1;i>=0;i--)
            cout << answer[i] <<" ";
        cout << "</s>";
    }
	
	return 0;
}


