#ifndef Bayes_H
#define Bayes_H
#include "MatrixOpe.h"

struct Par;

typedef vector<string> DIC;

DIC createVocabList(vector<DataStr> dataClass);//���ɵ����ֵ�

Matrix createFectVec(vector<DataStr> dataClass,DIC dic);//����ѵ����������

int trainNB(Matrix X,Matrix Y);//ѵ��������ʵ�֣�ע��Բ�������ƽ������

int classifyNB(Matrix testVecX);//������ߺ���

void getAllFiles(string path, vector <string>&files);

void Bayes();

#endif

