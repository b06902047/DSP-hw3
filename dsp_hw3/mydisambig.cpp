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

map<string,vector<string>> dict;
ofstream foutput;
double viter[3000][3000]={0};
int trace[3000][3000]={0};

void Dealdiction(char* ztob){
    ifstream mapping;
    mapping.open(ztob);
    string line2;
    while(getline (mapping, line2)){//deal with map
	    string zhu;
        zhu.assign(line2.begin(),line2.begin()+2);
       for(int i=1;i<(line2.size()+1)/3;i++){
        string help;
        help.assign(line2.begin()+3*i,line2.begin()+3*i+2);
        dict[zhu].push_back(help);
       }
    }
    mapping.close();
}
double probab(const char* word1,const char* word2,Ngram &lm,Vocab &voc){//P(word2|word1)
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
    char seg_file[256];
    char ztob[256];
    char lmodel[256];
    char outputfile[256];
    strcpy(seg_file, argv[1]);
    strcpy(ztob, argv[2]);
    strcpy(lmodel, argv[3]);
    strcpy(outputfile, argv[4]);
    foutput.open(outputfile);
    int order=2;
    Vocab voc;
    Ngram lm(voc,order);
    File lmFile( lmodel, "r" );
    lm.read(lmFile);
    lmFile.close();
    Dealdiction(ztob);
    /*for(map<string,vector<string>>::iterator iter = dict.begin(); iter != dict.end() ; iter++){
		foutput << iter->first<<" " <<iter->second[0]<<endl;
	}*/
    ifstream test_data;
    test_data.open(seg_file);//deal with each line
    string line;
    while(getline(test_data,line)){//處理每一行
        auto itor = remove_if(line.begin(), line.end(), ::isspace);
        line.erase(itor, line.end());//去除空白
        string word;
        word.assign(line.begin(),line.begin()+2);
        for(int i=0;i<dict[word].size();i++){
                viter[i][0]=probab("<s>",dict[word][i].c_str(),lm,voc);  
        }
        for(int i=1;i<line.size()/2;i++){//橫軸
            string prev=word;
            //foutput<<prev<<endl;
            word.assign(line.begin()+2*i,line.begin()+2*i+2);
            for(int j=0;j<dict[word].size();j++){//縱軸
                double maxprob=-1000000000;
                int maxdex=-1;
                for(int k=0;k<dict[prev].size();k++){
                    double now=probab(dict[prev][k].c_str(),dict[word][j].c_str(),lm,voc)+viter[k][i-1];//problem
                    if(now>maxprob){
                        maxprob=now;
                        maxdex=k;
                    }
                }
                viter[j][i]=maxprob;
                trace[j][i]=maxdex;
            }
        }
        string prev=word;
        double maxprob=-1000000000;int maxdex=-1;
        for(int i=0;i<dict[prev].size();i++){
            double now=probab(dict[prev][i].c_str(),"</s>",lm,voc)+viter[i][line.size()/2-1];
            if(now>maxprob){
                maxprob=now;
                maxdex=i;
            }
        }
        vector<string> answer;
        int index=maxdex;
        for(int i=line.size()/2-1;i>=0;i--){
            string help;
            help.assign(line.begin()+2*i,line.begin()+2*i+2);
            //foutput << help <<endl;
            answer.push_back(dict[help][index]);
            index=trace[index][i];
        }
        foutput << "<s> ";
        for(int i=line.size()/2-1;i>=0;i--)
            foutput << answer[i] <<" ";
        foutput << "</s>"<<endl;
    }
    
    return 0;
}
