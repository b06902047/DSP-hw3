#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include "Ngram.h"
#include "Vocab.h"
#include "LM.h"

#define PB push_back
#define MAXWORD 3000
#define MAXLEN 100
#define INF 1e9

using namespace std;

double getBigramProb(const char *w1, const char *w2, Ngram &lm, Vocab &voc){
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}

int main(int argc, char *argv[]){
	// arg parse
	if (argc < 9){
		cout << "Usage ./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2" << endl;
	}
	int ngram_order = 0;
	string text_fname, map_fname, lm_fname;
	for (int i = 0; i < argc-1; i++){
		if (!strcmp("-text", argv[i]))		text_fname = argv[i+1];
		if (!strcmp("-map", argv[i]))		map_fname = argv[i+1];
		if (!strcmp("-lm", argv[i]))		lm_fname = argv[i+1];
		if (!strcmp("-order", argv[i]))		ngram_order = atoi(argv[i+1]);
	}
	Vocab voc;
	Ngram lm(voc, ngram_order);
	string line;
	// read language model
	File lmFile(lm_fname.c_str(), "r" );
	lm.read(lmFile);
	lmFile.close();

	// read mapping file to map
	map <string, vector<string> > mapping;
	ifstream mapfile;
	mapfile.open(map_fname.c_str());
	while(getline(mapfile, line)){
		string zhuyin;
		string word;
		zhuyin.assign(line.begin(), line.begin()+2);
		int slen = line.size();
		for (int i = 3; i < slen; i+=3){
			word.assign(line.begin()+i, line.begin()+i+2);
			mapping[zhuyin].PB(word);
		}
	}
	mapfile.close();

	// read text-file x decode zhuyin
	ifstream textfile;
	textfile.open(text_fname.c_str());
	while(getline(textfile, line)){
		line.erase(remove(line.begin(), line.end(), ' '), line.end());	// erase space
		int sen_len = line.size() / 2;
		// viterbi alog.
		double delta[MAXLEN][MAXWORD] = {{0.0}};	// log prob.
		int idxtable[MAXLEN][MAXWORD] = {{0}};		// backtracking idx table
		string zhuyin;
		zhuyin.assign(line.begin(), line.begin()+2);
		// cout << "zhuyin = " << zhuyin << endl;
		
		// initialize
		int N = mapping[zhuyin].size();
		for (int i = 0; i < N; i++){
			delta[0][i] = getBigramProb("<s>", mapping[zhuyin][i].c_str(), lm, voc);
		}
		string prev_zhuyin = zhuyin;
		int preN = N;
		// dynamic programming
		for (int t = 1; t < sen_len; t++){
			zhuyin.assign(line.begin()+2*t, line.begin()+2*t+2);
			int N = mapping[zhuyin].size();
			for (int j = 0; j < N; j++){
				double max_prob = -INF;
				int max_idx = 0;
				for (int i = 0; i < preN; i++){
					double cur_prob = getBigramProb(mapping[prev_zhuyin][i].c_str(), mapping[zhuyin][j].c_str(), lm, voc) + delta[t-1][i];
					if (cur_prob > max_prob){
						max_prob = cur_prob;
						max_idx = i;
					}
				}
				delta[t][j] = max_prob;
				idxtable[t][j] = max_idx;
			}
			preN = N;
			prev_zhuyin = zhuyin;
		}
		// eos idx
		int max_idx;
		double max_prob = -INF;
		for (int i = 0; i < preN; i++){
			double cur_prob = getBigramProb(mapping[prev_zhuyin][i].c_str(), "</s>", lm, voc) + delta[sen_len-1][i];
			if (cur_prob > max_prob){
				max_prob = cur_prob;
				max_idx = i;
			}
		}
		// backtracking
		vector<string> sentence;
		for (int t = sen_len-1; t >= 0; t--){
			zhuyin.assign(line.begin()+2*t, line.begin()+2*t+2);
			sentence.PB(mapping[zhuyin][max_idx]);
			if (t > 0){
				max_idx = idxtable[t][max_idx];
			}
		}
		// output sentence
		reverse(sentence.begin(), sentence.end());
		cout << "<s>";
		for (int i = 0; i < sentence.size(); i++){
			cout << " " << sentence[i];
		}
		cout << " </s>" << endl;
	}
	return 0;
}
