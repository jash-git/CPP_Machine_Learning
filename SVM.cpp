#include "SVM.h"
#define MAX 1000000
#define MIN -100000
//SMO�����ṹ��
int SVM()
{
    ///���������ļ����浽����dtm�ľ���Ԫ����

    ///ͨ����������е�load������ʼ��������������������x,y
    //char file[20]="data\\svm.txt";
    Matrix x;
    //x.LoadData(file);

    char file[20]="data\\svm.txt";
    x.LoadData(file);
    //x = x.transposeMatrix();

    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);
    for(int i=x.row-1;i>100;i--)
    //x.deleteOneCol(x.col-1);
        x.deleteOneRow(i);
    cout<<x.row<<"*"<<x.col<<endl;
    cout<<y.row<<"*"<<y.col<<endl;
    ///����SMO�������SVMģ��
    cout<<"SVM"<<endl;
    SMOP smop;
    kTup ktup;//�˺����Ķ��壬����typeԪ��Ϊ0��ʾ�����ú˺�������0�ֱ��Ӧ��ͬ�ĺ˺���
    ktup.type=1;
    ktup.arg=1.0;
    smop.smoP(x,y,0.6,0.001,40,ktup);//
    return 0;
}


