/***
��������ID3,C4.5,CART

����

1��Ϣ����,��Ϣ������,����ָ��
2�������ͣ�string��
3Ԥ��֦
**/

#include "MatrixOpe.h"
#define MAX_SIZE_OF_TRAINING_SET 1000
using namespace std;

struct Tree;

vector<string> getkindattr(const DataStr &data,int axis);//���axis���Ե����в�ͬȡֵ

double calcShannonEntOrGini(const DataStr &data, const string &type);

DataStr splitDataSet(const DataStr &data,int axis,string value);

int chooseBestFectureTosplit(const DataStr &data, const string &type, double epsilon, int minLen);

Tree dataToTree(const DataStr &data,const string &type, int bbestFet);

int createTree();

int DTree(const string &type);
