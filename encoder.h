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
    char c;//ascii ����
    char code[100] = { 0 };//ascii ������ huffman code
    double p;//��
    Node* leftchild;
    Node* rightchild;
    Node* nextnode;//Q ������ ���� next node
    Node(Node* A, Node* B)//Node ������, �� ��带 ���� ���ο� ��带 ������
    {
        c = NULL;
        this->p = A->p + B->p;
        leftchild = A;
        rightchild = B;
        nextnode = NULL;
    }
    Node(char c, double p)//Node ������, ascii ���ڸ� �������� ��带 ���� ���
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
    Node* head;//Q�� ���
    int node_c;//node�� ����
    myQ()
    {
        head = NULL;
        node_c = 0;
    }
    void enQ(Node* node)//node ����, ������������ ����
    {
        if (head == NULL)//head�� ���� ���
        {
            head = node;
            node_c++;
            return;
        }
        //head�� ���� ���
        Node* pCur = head;
        Node* prev = head;
        while ((node->p >= pCur->p))//�������� Ž��, node�� �󵵰� pCur�� �󵵺��� ũ�� ��� Ž��, pCur�� NULL�̸� ����
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
char* makebinary(int n);//10���� -> 2����
int makeint(char* bin);//2���� -> 10����

void inorder(Node* root, char* code, Node** table, int& tableidx);

void encoder();