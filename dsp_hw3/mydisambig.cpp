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
       for(int i=1;i<strlen(line)/3;i++){
        string help;
        help.assign(line.begin()+3*i,line.begin()+3*i+2)
        dict[zhu].push_back(help);
       }
    }
    mapping.close();
}
double probab(char* word1,char* word2){//P(word2|word1)
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
    while(getline(testdata,line)){//處理每一行
        double viter[15000][15000]={{0.0}};
        int trace[15000][15000]={{0}};
        auto itor = remove_if(line.begin(), line.end(), ::isspace);
        line.erase(itor, str.end());
        string word;
        word.assign(line.begin(),line.begin()+2);
        for(int i=0;i<dict[word].size();i++)
            viter[i][0]=probab("<s>",dict[word][i].c_str());
        string prev(word);
        for(int i=1;i<line.size()/2;i++){
            
        }
    }
	
	return 0;
}


