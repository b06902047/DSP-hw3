#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function
import codecs, sys
import json

def main():
	dic ={'ㄅ':[],'ㄆ':[],'ㄇ':[],'ㄈ':[],'ㄉ':[],'ㄊ':[],'ㄋ':[],'ㄌ':[],'ㄍ':[],'ㄎ':[],\
'ㄏ':[],'ㄐ':[],'ㄑ':[],'ㄒ':[],'ㄓ':[],'ㄔ':[],'ㄕ':[],'ㄖ':[],'ㄗ':[],'ㄘ':[],'ㄙ':[],\
'ㄧ':[],'ㄨ':[],'ㄩ':[],'ㄚ':[],'ㄛ':[],'ㄜ':[],'ㄝ':[],'ㄞ':[],'ㄟ':[],'ㄠ':[],'ㄡ':[],\
'ㄢ':[],'ㄣ':[],'ㄤ':[],'ㄥ':[],'ㄦ':[]}
	inputfile=sys.argv[1]
	outputfile=sys.argv[2]
	inFile = codecs.open("Big5-ZhuYin.map", "rb", "big5-hkscs",'replace')
	outFile = codecs.open("ZhuYin-Big5.map", "wb","big5-hkscs",'replace')
	content=inFile.readlines()#unicode
	inFile.close()
	for j in content:
		j=j.split(" ")
		j0=j[0]	#j[0]=>字
		j1=j[1].split("/")	#j[1]=>[ㄐㄧㄝ_, ㄒㄧㄝˊ]
		for k in j1:
			dic[k[0].encode("utf-8")].append(j0.encode("utf-8"))
	for i in dic.keys():
		helpme="{}".format(str(i).decode('string_escape')).decode("utf-8")
		outFile.write(helpme)
		for j in dic[i]:
			helpme=" {}".format(str(j).decode('string_escape')).decode("utf-8")
			outFile.write(helpme)
		outFile.write('\n')
		for j in dic[i]:
			helpme="{} {}\n".format(str(j).decode('string_escape'),str(j).decode('string_escape')).decode("utf-8")
			outFile.write(helpme)
	
	#inFile = codecs.open("ZhuYin-Big5.map", "rb","big5-hkscs",'replace')
	'''
	for i in range(1000):
		content=inFile.readline()#unicode
		print(content.encode("utf-8"),end='')
	'''
	
if __name__ == '__main__':
    main()



