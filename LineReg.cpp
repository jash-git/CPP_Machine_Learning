#include "MatrixOpe.h"
/**

���Իع麯����ʵ�֣�����һ������Իع飬��Сƽ������Ϊ��ʧ��������Ŀ�꺯����һ����Լ����͹���ι滮���⣬
��͹���ι滮����ļ�Сֵ�ڵ���Ϊ0��ȡ�����Ҽ�СֵΪȫ����Сֵ�����б�ʽ�⡣������ѧ���ʽʵ�־���֮���������ò���w��
**/
int regression(Matrix x,Matrix y)
{
    Matrix xT=x.transposeMatrix();
    Matrix xTx=xTx.multsMatrix(xT,x);
    Matrix xTx_1=xTx.niMatrix();
    Matrix xTx_1xT=xTx_1xT.multsMatrix(xTx_1,xT);
    Matrix ws;
    ws=ws.multsMatrix(xTx_1xT,y);
    cout<<"ws"<<endl;
    ws.print();
    return 0;
}
/**
�������ع麯��ֻ����һ������Իع麯���Ļ������ڶԽ�������������ĸ�������н�����󲻿�������ԣ�ͬ��Ҳ���������Ŀ�ģ�
���ó��õĶ������͵õ���ֱ������lam����ʽ��
**/

int ridgeRegres(Matrix x,Matrix y,double lam)
{
    Matrix xT=x.transposeMatrix();
    Matrix xTx=xTx.multsMatrix(xT,x);
    Matrix denom(xTx.row,xTx.col,lam,"diag");
    xTx=xTx.addMatrix(xTx,denom);
    Matrix xTx_1=xTx.niMatrix();
    Matrix xTx_1xT=xTx_1xT.multsMatrix(xTx_1,xT);
    Matrix ws=ws.multsMatrix(xTx_1xT,y);
    cout<<"ws"<<endl;
    ws.print();
    return 0;
}

/**
�ֲ���Ȩ���Իع��������Իع�Ļ����϶�ÿһ������������ѵ����ʱ�����ÿһ��ѵ���������������е���������һ����Ȩ��ϣ�
Ȩ�ص�ȷ������ͨ��һ���������㣬���õ��и�˹�ˣ����������Խ����Ȩ��Խ�󣬷�֮ԽС����������ÿһ�����������͵õ��˲�һ����
Ȩ���������������ó���������߲��������Ե��ˣ����������ӵ�ģ�͵ĸ��Ӷ������õ���Ϸ��������ݡ�

**/
//��Ҫע����Ǿֲ���Ȩ���Իع��Ƕ�ÿһ����������Ȩ�ؼ��㣬���Զ���ÿһ����������һ��Ȩ��w����������ĺ���ֻ�Ǿֲ����Իع��һ����Ҫ��������
Matrix locWeightLineReg(Matrix test,Matrix x,Matrix y,const double &k)
{
    Matrix w(x.row,x.row,0,"T");
    double temp=0;
    int i,j;

    /**
    ���ݲ��������������������ľ����Ѿ�ѡ��ĺ�ȷ���ֲ���Ȩ���󣬲��öԽ�����Ϊ�ֲ���Ȩֵ
    **/
    for(i=0;i<x.row;i++)
    {
        temp=0;
        for(j=0;j<x.col;j++)
        {
            temp+=(test.data[0][j]-x.data[i][j])*(test.data[0][j]-x.data[i][j]);
        }
        w.data[i][i]=exp(temp/-2.0*k*k);
    }
    Matrix xT=x.transposeMatrix();
    Matrix wx=wx.multsMatrix(w,x);
    Matrix xTwx;
    xTwx=xTwx.multsMatrix(xT,wx);
    Matrix xTwx_1;
    xTwx_1=xTwx.niMatrix();
    Matrix xTwx_1xT;
    xTwx_1xT=xTwx_1xT.multsMatrix(xTwx_1,xT);
    Matrix xTwx_1xTw;
    xTwx_1xTw=xTwx_1xTw.multsMatrix(xTwx_1xT,w);
    Matrix ws = xTwx_1xTw * y;
    return ws;
}
/**
ͨ����������ľֲ���Ȩ���Իع麯��ʵ�ֶ�ÿһ����������ȷ�����Ĳ�����ͬ��������������ʽ�洢����n��������������n�У���m��������һ��m�У�
**/

int lwlrtest(Matrix x,Matrix y,const double &k)
{
    int i,j;
    Matrix test;
    Matrix wsOne;
    Matrix ws;
    ws=x.copyMatrix();
    /**
    ÿһ�������������þֲ���Ȩ���Իع�
    **/
    for(i=0;i<x.row;i++)///ws����Ϊһ��Ȩ�ز���������m�б�ʾm������
    {
        test=x.getOneRow(i);
        wsOne=locWeightLineReg(test,x,y,k);
        for(j=0;j<x.col;j++)
        {
            ws.data[i][j]=wsOne.data[0][j];
        }
    }
    for(i=0;i<ws.row;i++)
    {
        for(j=0;j<ws.col;j++)
        {
            cout<<ws.data[j][i]<<"  ";
        }
        cout<<endl;
    }

    /**��֤�ֲ���Ȩ���Իع����ȷ��*/
    double yy;
    for(i=0;i<ws.row;i++)
    {
        yy=0;
        for(j=0;j<ws.col;j++)
        {
            yy+=ws.data[j][i]*x.data[i][j];
        }
        cout<<"y="<<yy<<endl;
    }
    return 0;
}

int LineReg()
{
    const char *file="data\\lineReg.txt";
    const string model="regression";
    const double arg=0.01;
    Matrix x;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    x.LoadData(file);
    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);
    if(model=="regression")
    {
        regression(x,y);
    }
    if(model=="ridgeRegres")
    {
        ridgeRegres(x,y,0.01);
    }
    if(model=="lwlrtest")
    {
        lwlrtest(x,y,1);
    }
    return 0;
}
