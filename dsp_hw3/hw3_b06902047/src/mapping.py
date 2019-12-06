#!/usr/bin/env python3
#coding=Big5
from __future__ import print_function
import codecs, sys
import json

def main():
	dic ={'�t':[],'�u':[],'�v':[],'�w':[],'�x':[],'�y':[],'�z':[],'�{':[],'�|':[],'�}':[],\
'�~':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],\
'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],'��':[],\
'��':[],'��':[],'��':[],'��':[],'��':[]}
	inFile = codecs.open(sys.argv[1], "rb", "big5-hkscs",'replace')
	outFile = codecs.open(sys.argv[2], "wb","big5-hkscs",'replace')
	content=inFile.readlines()#unicode
	inFile.close()
	for j in content:
		j=j.split(" ")
		j0=j[0]	#j[0]=>�r
		j1=j[1].split("/")	#j[1]=>[������_, ��������]
		for k in j1:
			dic[k[0]].append(j0)
	for i in dic.keys():
		helpme="{}".format(i)
		outFile.write(helpme)
		for j in dic[i]:
			helpme=" {}".format(j)
			outFile.write(helpme)
		outFile.write('\n')
		for j in dic[i]:
			helpme="{} {}\n".format(j,j)
			outFile.write(helpme)
	
	#inFile = codecs.open("ZhuYin-Big5.map", "rb","big5-hkscs",'replace')
	'''
	for i in range(1000):
		content=inFile.readline()#unicode
		print(content.encode("utf-8"),end='')
	'''
	
if __name__ == '__main__':
    main()
