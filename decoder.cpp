//#include"encoder.h"
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<math.h>
#include"decoder.h"
using namespace std;


char* Dmakebinary(int input)//decimal -> 8bit binary
{
	char bin[8];
	char result[9];
	int idx = 7;
	int n = input;
	while (idx >= 0)
	{
		if (n % 2 == 0) bin[idx] = '0';
		else bin[idx] = '1';
		n /= 2;
		idx--;
	}
	for (int i = 0; i < 8; i++)
		result[i] = bin[i];
	result[8] = 0;
	if (input < 0)//if input is negative
	{
		for (int i = 0; i < 8; i++)//make 1's complement
		{
			if (result[i] == '0') result[i] = '1';
			else result[i] = '0';
		}
		int j = 7;
		while (j>-1)//make 2's complement from 1's complement
		{
			if (result[j] == '1') result[j] = '0';
			else
			{
				result[j] = '1';
				break;
			}
			j--;
		}
	}
	
	return result;
}
int Dmakeint(char* bin)//binary -> decimal
{
	int i = strlen(bin);
	int result = 0;
	int j = 0;
	int bi;//bit indext
	while (i > 0)
	{
		bi = pow(2, j);//2^j
		result += bi * ((bin[i - 1]) - 48);//'0' = ascii 48, '1' = ascii 49
		j++;
		i--;
	}
	return result;
}
void decoder()
{
	FILE* table, * code;
	table = fopen("huffman_table.hbs", "rb");//huffman_table.hbs open
	code = fopen("huffman_code.hbs", "rb");//huffman_code.hbs open
	char* dec_table = new char[1000000];//make table string
	char* dec_code = new char[1000000];//make encoded string
	memset(dec_table, 0, 1000000);//fill with zeros
	memset(dec_code, 0, 1000000);//fill with zeros
	char table_char;//to get 1byte character from table
	while (!feof(table))
	{
		table_char = fgetc(table);
		//if (table_char == NULL) break;
		strcat(dec_table, Dmakebinary(table_char));
	}
	while (!feof(code))
	{
		table_char = fgetc(code);
		//if (table_char == NULL) break;
		strcat(dec_code, Dmakebinary(table_char));
	}
	int table_l = strlen(dec_table);
	huffman_char huf_table[128];
	int table_idx = 0;
	while (1)//make huffman table class from dec_table
	{
		if (!strncmp(dec_table, "1010101010101", 13))
			break;

		char ascii_char[9] = {0};//ascii character
		char code_length[9] = {0};//length of code
		char huf_code[100] = {0};//huffman code

		strncpy(ascii_char, dec_table, 8);//get ascii character
		dec_table += 8;
		
		strncpy(code_length, dec_table, 8);//get length of code
		dec_table += 8;

		strncpy(huf_code, dec_table, Dmakeint(code_length));//get code
		dec_table += strlen(huf_code);
		huf_table[table_idx].c = Dmakeint(ascii_char);
		huf_table[table_idx].length = Dmakeint(code_length);
		strcpy(huf_table[table_idx].code, huf_code);
		table_idx++;
	}
	char* output_string = new char[1000000];
	memset(output_string, 0, 1000000);//fill with zeros
	int string_idx = 0;
	while (dec_code)//make output string
	{
		if (!strncmp(dec_code, "1010101010101", 13)) break;
		for (int i = 0; i < table_idx; i++)
		{
			if (!strncmp(dec_code, huf_table[i].code, huf_table[i].length))//find matched ascii character from table
			{
				output_string[string_idx++] = huf_table[i].c;
				dec_code += huf_table[i].length;
				break;
			}
		}
	}
	ofstream output;
	output.open("output.txt");
	output << output_string;
}