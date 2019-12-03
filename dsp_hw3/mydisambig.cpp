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
char ztob[256];
char lmodel[256];

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
    map<char*,vector<char*>> dict;
    FILE *mapping=fopen(ztob,"r");
    char zbmap;
    char line[1024];
    while(!feof(mapping)){
    	fgets(line, sizeof(line), mapping);
    	char zhu[3];
    	char word[3];
    	zhu[2]='\0';
    	strncpy(zhu,line,2);
    	vector<char*> vec;
    	dict.insert(pair<char*, vector<char*>>(zhu, vec));
    	for(int i=1;i<sizeof(line)/2;i++){
    		char help[3];
    		help[2]='\0';
    		strncpy(help,line+3*i,2);
    		dict[zhu].push_back(help);
    	}
    }
    map<char*, vector<char*>>::iterator iter;
    for(iter = dict.begin(); iter != dict.end(); iter++)
            cout<< iter->first << endl;

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




