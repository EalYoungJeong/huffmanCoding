#pragma once
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<math.h>
using namespace std;

class ASCII_char//class that save the information of ascii character
{
public:
    char c;//number of ascii character
    int count;//count of ascii charater
    ASCII_char()
    {
        c = NULL;
        count = 0;
    }
};
class Node
{
public:
    char c;//ascii 문자
    char code[100] = { 0 };//ascii 문자의 huffman code
    double p;//빈도
    Node* leftchild;
    Node* rightchild;
    Node* nextnode;//Q 관리를 위한 next node
    Node(Node* A, Node* B)//Node 생성자, 두 노드를 합쳐 새로운 노드를 만들경우
    {
        c = NULL;
        this->p = A->p + B->p;
        leftchild = A;
        rightchild = B;
        nextnode = NULL;
    }
    Node(char c, double p)//Node 생성자, ascii 문자를 기준으로 노드를 만들 경우
    {
        this->c = c;
        this->p = p;
        leftchild = NULL;
        rightchild = NULL;
        nextnode = NULL;
    }
};
class myQ
{
public:
    Node* head;//Q의 헤드
    int node_c;//node의 개수
    myQ()
    {
        head = NULL;
        node_c = 0;
    }
    void enQ(Node* node)//node 삽입, 오름차순으로 정렬
    {
        if (head == NULL)//head가 없을 경우
        {
            head = node;
            node_c++;
            return;
        }
        //head가 있을 경우
        Node* pCur = head;
        Node* prev = head;
        while ((node->p >= pCur->p))//오름차순 탐색, node의 빈도가 pCur의 빈도보다 크면 계속 탐색, pCur이 NULL이면 중지
        {
            prev = pCur;
            pCur = pCur->nextnode;
            if (!pCur) break;
        }
        if (head == pCur)// if node has minimum probability, head is node 
        {
            head = node;
            node->nextnode = pCur;
        }
        else
        {
            prev->nextnode = node;
            node->nextnode = pCur;
        }
        node_c++;
    }
    Node* deQ()//dequeue
    {
        Node* deQ = head;
        head = deQ->nextnode;
        node_c--;
        return deQ;
    }
};
char* makebinary(int n);//10진수 -> 2진수
int makeint(char* bin);//2진수 -> 10진수

void inorder(Node* root, char* code, Node** table, int& tableidx);

void encoder();