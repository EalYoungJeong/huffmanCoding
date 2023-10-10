#pragma once
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<math.h>

class huffman_char
{
public:
	char c;
	int length;
	char code[100] = { 0 };
	/*huffman_char(int c, int length, char* code)
	{
		this->c = (char)c;
		this->length = length;
		strcpy(this->code, code);
	}*/
};
char* Dmakebinary(int n);//10진수 -> 2진수
int Dmakeint(char* bin);//2진수 -> 10진수
void decoder();