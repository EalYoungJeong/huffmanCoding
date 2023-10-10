#include"encoder.h"
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<math.h>
using namespace std;

char* makebinary(int n)//10진수 -> 2진수
{
    char bin[8];
    char result[9];
    int idx = 7;
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
    return result;
}
int makeint(char* bin)//2진수 -> 10진수
{
    int i = strlen(bin);
    int result = 0;
    int j = 0;
    int bi;//bit indext
    while (i > 0)
    {
        bi = pow(2, j);//2의 j제곱
        result += bi * ((bin[i - 1]) - 48);//'0' = ascii 48, '1' = ascii 49
        j++;
        i--;
    }
    return result;
}
void inorder(Node* root, char* code, Node** table, int& tableidx)
{
    if (root->c == NULL)//ascii 가 null이면 ascii node가 아닌 새로만든 임시 노드
    {
        char tempcode[100] = "";
        char ttempcode[100] = "";
        strcpy(tempcode, code);
        strcpy(ttempcode, code);
        strcat(tempcode, "0");//코드 마지막 bit에 0 대입
        inorder(root->leftchild, tempcode, table, tableidx);//leftchild 이동
        strcat(ttempcode, "1");//코드 마지막 bit에 0 대입
        inorder(root->rightchild, ttempcode, table, tableidx);//right child 이동
    }
    else//ascii node
    {
        strcpy(root->code, code);//코드 복사
        table[tableidx] = root;//table에 입력
        tableidx++;//table 인덱스 증가
    }
}
void encoder()
{
    ifstream input_data;
    input_data.open("input_data.txt");//open input data.txt
    ASCII_char ASCII_c[128];
    for (int i = 0; i < 128; i++)//i is ascii number of character
        ASCII_c[i].c = i;
    int all_char_c = 0;//count of all characters in input_data.txt

    while (!input_data.eof())//get information about ASCII characters in input_data.txt 
    {
        char oneline[300] = { 0 };//array that hold one line of string 
        input_data.getline(oneline, 300);//get one line
        oneline[strlen(oneline)] = '\n';//input enter character in last point of string
        for (int i = 0; i < strlen(oneline); i++)
        {
            ASCII_c[oneline[i]].count++; //*1 to count of each ascii character 
            all_char_c++;//count of all characters +1
        }
    }
    input_data.close();//close input data.txt file
    char nl = '\n';
    ASCII_c[(int)nl].count--;//-1 the number of new line character
    all_char_c--;//-1 the number of all characters

    //buble sort array ASCII_c by count of each characters

    myQ Q;//linked list that manages priority queue about huffam tree node
    for (int i = 0; i < 128; i++)//loop that make priority queue
    {
        if (ASCII_c[i].count != 0)//if the number of ascii character is not zero,
        {
            Node* node = new Node(ASCII_c[i].c, ASCII_c[i].count);//make new node with ASCII_c class
            Q.enQ(node);//input node in Q
        }
    }

    //make huffman tree
    while (Q.node_c >= 2)
    {
        Node* Anode = Q.deQ();//dequeue two nodes that have minimum frequency
        Node* Bnode = Q.deQ();
        Node* node = new Node(Anode, Bnode);//make new node with two dequeued nodes
        Q.enQ(node);//input new node in Q
    }

    Node* root = Q.head;//root of huffman tree
    Node* table[100];//array that saves node that has ascii character
    int table_idx = 0;//index of array table
    char code[100] = { 0 };//string to make huffman code about each character in input_data.txt
    inorder(root, code, table, table_idx);//inorder travel, take node into array table


    //buble sort array table by ascii character number
    for (int i = 0; i < table_idx - 1; i++)
    {
        for (int j = 0; j < table_idx - 1; j++)
        {
            if (table[j]->c > table[j + 1]->c)
            {
                Node* temp = table[j + 1];
                table[j + 1] = table[j];
                table[j] = temp;
            }
        }
    }
    for (int i = 0; i < table_idx/2; i++)
    {
        cout << "'" << table[i]->c << "'" << "      " << table[i]->code << "                " << "'" << table[i + table_idx / 2]->c << "'" << "      " << table[i + table_idx / 2]->code << endl;
    }
    char* huf_table = new char[1000000];
    memset(huf_table, 0, sizeof(huf_table));//Initialize the array to zero
    int table_l = 0;//length of table
    for (int i = 0; i < table_idx; i++)//make huf_table
    {
        char asciicode[9];
        char codelength[9];
        strcpy(asciicode, makebinary((int)table[i]->c)); //make ascii charater to 8bit binary number
        strcpy(codelength, makebinary(strlen(table[i]->code)));//make length of code to 8bit binary number
        strcat(huf_table, asciicode);//put ascii code into huf_table string
        strcat(huf_table, codelength);//put only the lower 5bits of codelength into huf_table string
        strcat(huf_table, table[i]->code);//put huffman code to into huf_table string
        table_l += 16;//ascii character 8bit + code length 5bit
        table_l += strlen(table[i]->code);//+ length of code
    }
    strcat(huf_table, "1010101010101");//put EOD bit into huf_table
    table_l += 13;// + length of EOD
    int stuffing_c = 8 - (table_l % 8);//get the number of zeros to add to make a byte
    if (stuffing_c != 8)//add 0 as much as the number of stuffing bits to huf_table
    {
        for (int i = 0; i < stuffing_c; i++)
        {
            strcat(huf_table, "0");//add 0 in huf_table
            table_l++;//table length ++
        }
    }
    
    //////////////////// make huffman_table.hbs ///////////////////////////////
    FILE* h_table;
    h_table = fopen("huffman_table.hbs", "wb");
    char for_table[9] = { 0 };//get ibyte character from 8bit 
    for (int i = 0; i < table_l; i++)
    {
        for (int j = 0; j < 8; j++)
            for_table[j] = huf_table[i++];//divide huf_table string into 8bit
        i--;
        fputc(makeint(for_table), h_table);//make 8bit to 1byte number and put into huffman_table.hbs
    }
    fclose(h_table);

    //////////////////// make huffman_code.hbs ////////////////////////////////
    input_data.open("input_data.txt");//open input data.txt
    char* encoding = new char[1000000];
    int encoding_c = 0;
    memset(encoding, 0, sizeof(encoding));
    while (!input_data.eof())
    {
        char line[256] = { 0 };
        input_data.getline(line, 256);
        line[strlen(line)] = '\n';
        for (int i = 0; i < strlen(line); i++)
        {
            for (int j = 0; j < table_idx; j++)
            {
                if (line[i] == table[j]->c)
                {
                    strcat(encoding, table[j]->code);//input matched code in encoding array
                    encoding_c += strlen(table[j]->code);
                    break;
                }
            }
        }
    }
    int newline_l = 0;//code length of "\n"
    for (int i = 0; i < table_idx; i++)//get code length of "\n"
    {
        if (table[i]->c == '\n')
        {
            newline_l = strlen(table[i]->code);
            break;
        }
    }
    while (newline_l > 0)//delete last "\n" code in encoding string
    {
        encoding[encoding_c - newline_l] = 0;
        encoding_c--;
        newline_l--;
    }
    strcat(encoding, "1010101010101");//put EOD bit into huf_code
    encoding_c += 13;// + length of EOD
    stuffing_c = 8 - (encoding_c % 8);//get the number of zeros to add to make a byte
    if (stuffing_c != 8)//add 0 as much as the number of stuffing bits to huf_code
    {
        for (int i = 0; i < stuffing_c; i++)
        {
            strcat(encoding, "0");//add 0 in huf_code
            encoding_c++;//encoding length ++
        }
    }
    FILE* h_code;
    h_code = fopen("huffman_code.hbs", "wb");
    char for_code[9] = { 0 };//get ibyte character from 8bit 
    int k = 0;
    while (k < encoding_c)
    {
        for (int j = 0; j < 8; j++)
            for_code[j] = encoding[k++];
        fputc(makeint(for_code), h_code);//make 8bit to 1byte number and put into huffman_code.hbs
    }
    fclose(h_code);
    delete[] huf_table;
    delete[] encoding;
}