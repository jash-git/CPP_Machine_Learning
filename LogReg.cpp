#include "MatrixOpe.h"

/**
�ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

**/
int gradAscent_Log(Matrix x,Matrix y)
{
    if(y.col!=1)
    {
        cout<<"logReg is two class"<<endl;
        return -1;
    }
    Matrix weights(x.col,y.col,0.1,"T");
    Matrix xT = x.transposeMatrix();

    float alpha=0.01;///��������
    float error=0;///��¼������
    int iter=0;
    int i,j;
    Matrix z(y.row,y.col,0,"T");//���ȷ������Ĵ�С
    Matrix grad(x.col,y.col,0,"T");
    for(iter=0; iter<5000; iter++)
    {
        z = x * weights;
        for(i=0; i<z.row; i++)
        {
            z.data[i][0]=sigmoid(z.data[i][0]);
        }
        z = y - z;
        error=0;
        for(i=0; i<x.row; i++)///ͳ�ƴ�����
            error+=z.data[i][0];
        //cout<<"error="<<error<<endl;
        //if(error<x.row/100 && error>x.row/100)///���ô�����С��һ��ֵʱ�˳�����
           // break;
        grad = xT * z;///���㸺�ݶȷ���
        //grad = (1.0/x.row) * grad;
        for(i=0; i<grad.row; i++)
            grad.data[i][0]*= alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
        weights = weights + grad;///�����ݶȷ�����һ������
    }

    /**
    ��֤�㷨����ȷ��
    **/
    int er1=0,er2=0;
    Matrix train=x * weights;
    cout<<"test"<<endl;
    for(i=0; i<y.row; i++)
    {
        if(train.data[i][0]>0)
        {
            cout<<1-y.data[i][0]<<endl;
            er1+=(1-y.data[i][0]);
        }
        else
        {
            cout<<0-y.data[i][0]<<endl;
            er2-=(0-y.data[i][0]);
        }
    }
    //cout<<"er1="<<er1<<endl;
    //cout<<"er2="<<er2<<endl;
}
/**
����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

**/
int stoGradAscent_Log(Matrix x,Matrix y)//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
{
    Matrix xOneRow(1,x.col,0,"T");
    Matrix xOneRowT(x.col,1,0,"T");

    Matrix weights(x.col,y.col,0.1,"T");
    Matrix z(1,y.col,0,"T");//���ȷ������Ĵ�С
    Matrix grad(x.col,y.col,0,"T");

    double alpha=0.001;///����
    double error;
    int i,j,c;
    for(c=0; c<1000; c++)
    {
        for(i=0; i<x.row; i++)
        {
            xOneRow=x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
            z = xOneRow * weights;
            z.data[0][0]=sigmoid(z.data[0][0]);
            z.data[0][0]=y.data[i][0]-z.data[0][0];
            xOneRowT = xOneRow.transposeMatrix();
            grad = xOneRowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
            for(j=0; j<grad.row; j++)
                grad.data[j][0]*=alpha;
            weights = weights + grad; ///����
        }
    }
    //��֤�㷨����ȷ��
    cout<<"test"<<endl;
    Matrix test = x * weights;
    for(i=0; i<y.row; i++)
    {
        if(test.data[i][0]>0)
        {
            cout<<1-y.data[i][0]<<endl;
        }
        else
        {
            cout<<0-y.data[i][0]<<endl;
        }
    }
}

int LogReg()
{
    const char *file="data\\LogReg.txt";
    const string model="gradAscent";
    const double alpha=0.01;
    Matrix x;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    x.LoadData(file);
    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);

    if(model=="gradAscent")
        gradAscent_Log(x,y);
    if(model=="stoGradAscent")
        stoGradAscent_Log(x,y);

    return 0;
}
