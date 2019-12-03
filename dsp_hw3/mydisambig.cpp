#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <fstream>
#include<string>
#include <map>
#include <vector>
#include"File.h"
#include"Ngram.h"
#include"Vocab.h"

using namespace std;

char seg_file[256];
char mapping[256];
char model[256];

int main(int argc, char *argv[]){
	strcpy(seg_file, argv[2]);
	strcpy(ztob, argv[4]);
	strcpy(lmodel, argv[6]);
	int order=stoi(argv[8]);
	Vocab voc;
    Ngram lm( voc, order );
    File lmFile( lmodel, "r" );
    lm.read(lmFile);
    lmFile.close();
    map<string,vector<string>> dict;
    File *mapping=fopen(ztob,"r");
    char zbmap;
    char line[1024];
    while(!feof(mapping)){
    	fgets(line, sizeof(line), mapping);
    	char zhu[2];
    	char word[2];
    }


	return 0;
}

/*VocabIndex wid = voc.getIndex("囧");
    if(wid == Vocab_None) {
        printf("No word with wid = %d\n", wid);
        printf("where Vocab_None is %d\n", Vocab_None);
    }

    wid = voc.getIndex("患者");
    VocabIndex context[] = {voc.getIndex("癮") , voc.getIndex("毒"), Vocab_None};
    printf("log Prob(患者|毒-癮) = %f\n", lm.wordProb(wid, context));
*/




