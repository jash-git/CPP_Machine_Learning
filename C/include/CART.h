/***

�ع��� CART

����

1��С������ ,��Ϻ������þ�ֵ������ ��Ϻ�����������ģ��
2������ֵ��ѡ���Ǵ����������ѡ�񣬲��ǰ�����ѡ��
2�������ͣ�string������ֵ
3Ԥ��֦�����֦
**/

#include "MatrixOpe.h"
struct twoSubData_C;

struct split_C;

typedef struct bitnode bitnode,*bitree;
twoSubData_C binSplitDataSet(const Data &data,const int &axis,const double &value);

double mean_C(const Data &data);

double MeanErr_C(const Data &data);

split_C chooseBestSplit(const Data &data,const double &minErr);

int createBinTree(bitree &t,Data &data);

int preorder(bitree &t);//�ݹ��������������

int prune(bitree &t,const Data &testData);

double predict(bitree t, const RowData &data);//bitree &t���������ã����ܸı�����

int CART();
