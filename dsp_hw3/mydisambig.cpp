#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <fstream>
#include<string>
#include <malloc.h>
#include <memory.h>
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
char line[15000];
char problem[15000];
Vocab voc;
Ngram lm(voc,order);

map<char*,vector<char*>> dict;
char* strtrimc(char* withspace){
    int num=0;
    char nospace[strlen(withspace)];
    for(int i=0;i<strlen(withspace);i++){
        if(withspace[i]!=' '){
            strncpy(&nospace[num],&withspace[i],1);
            num++;
        }
    }
    nospace[num]='\0';
    return nospace;
}
void Dealdiction(){
    FILE *mapping=fopen(ztob,"r");
    while(!feof(mapping)){//deal with map
        fgets(line, sizeof(line), mapping);
        char* zhu=new char[3];
        zhu[2]='\0';
        strncpy(zhu,line,2);
       for(int i=1;i<strlen(line)/3;i++){
            char* help=new char[3];
            help[2]='\0';
            strncpy(help,line+3*i,2);
            dict[zhu].push_back(help);
       }
    }
    fclose(mapping);
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
    //map<char*, vector<char*>>::iterator iter;
    FILE* test_data=fopen(seg_file,"r");//deal with each line
    while(!feof(test_data)){//處理每一行
        double viter[15000][15000]={{0.0}};
        int trace[15000][15000]={{0}};
        char* c =fgets(line, sizeof(line), test_data);
        strcpy(problem,strtrimc(line));//problem=>沒有空白的string
        if(problem[strlen(line)-1]=='\n') problem[strlen(line)-1]='\0';
        char word[3];word[2]='\0';
        strncpy(word,line,2);
        for(int i=0;i<dict[word].size();i++){
            viter[i][0]=probab("<s>",dict[word][i]);
        }
        for(int i=1;i<strlen(line)/2;i++){
            char word[3];word[2]='\0';
            strncpy(word,line+i*2,2);
        }
    }
	
	return 0;
}

/*
 if(wid == Vocab_None) {
        printf("No word with wid = %d\n", wid);
        printf("where Vocab_None is %d\n", Vocab_None);
    }

    wid = voc.getIndex("患者");
    VocabIndex context[] = {voc.getIndex("癮") , voc.getIndex("毒"), Vocab_None};
    printf("log Prob(患者|毒-癮) = %f\n", lm.wordProb(wid, context));
*/


