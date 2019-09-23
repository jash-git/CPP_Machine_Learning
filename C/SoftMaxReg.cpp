#include "SoftMaxReg.h"

/**
�ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

**/
int gradAscent_SoftMax(Matrix x,Matrix y)
{
    Matrix weights(x.col,y.col,0.1,"T");
    Matrix xT = x.transposeMatrix();

    float alpha=0.001;///��������
    float error=0;///��¼������
    int iter=0;
    int i,j;
    Matrix z(y.row,y.col,0,"T");//���ȷ������Ĵ�С
    Matrix zRowSum(z.row,1,0,"ss");//����ά����ͣ���һ��Ϊ������ʽ
    Matrix grad(x.col,y.col,0,"T");
    for(iter=0; iter<10000; iter++)
    {
        z = x * weights;
        for(i=0; i<z.row; i++)
        {
            zRowSum.data[i][0]=0;
            for(j=0;j<z.col;j++)
            {
                z.data[i][j]=sigmoid(z.data[i][j]);
                zRowSum.data[i][0]+=z.data[i][j];//���
            }
            for(j=0;j<z.col;j++)
            {
                z.data[i][j]/=zRowSum.data[i][0];//��һ��
                //if(iter%5000==0)
                    //cout<<z.data[i][j] <<"  ";
            }
            //if(iter%5000==0)
                //cout<<endl;
        }
        z = y - z;

        grad = xT * z;///���㸺�ݶȷ���
        //grad = (1.0/x.row) * grad;
        for(i=0; i<grad.row; i++)
        {
            for(j=0;j<grad.col; j++)
            {
                grad.data[i][j]*= alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
            }
        }
        weights = weights + grad;///�����ݶȷ�����һ������
        //weights.print();
    }
    /**
    ��֤�㷨����ȷ��
    **/
    Matrix test=x * weights;
    cout<<"test"<<endl;
    for(i=0; i<y.row; i++)
    {
        //for(j=0; j<y.col; j++)
        if(test.data[i][0]>test.data[i][1])
            cout<<0-y.data[i][1]<<"  ";
        else
            cout<<1-y.data[i][1]<<"  ";
        cout<<endl;
    }
}
/**
����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

**/
int stoGradAscent_SoftMax(Matrix x,Matrix y)//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
{
    Matrix xOneRow(1,x.col,0,"T");
    Matrix xOneRowT(x.col,1,0,"T");

    Matrix weights(x.col,y.col,0.1,"T");
    Matrix z(1,y.col,0,"T");//���ȷ������Ĵ�С
    Matrix grad(x.col,y.col,0,"T");
    double zRowSum=0;
    double alpha=0.001;///����
    double error;
    int i,j,k,iter;
    for(iter=0; iter<5000; iter++)
    {
        for(i=0; i<x.row; i++)
        {
            xOneRow=x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
            z = xOneRow * weights;
            zRowSum=0;
            for(j=0;j<z.col;j++)
            {
                z.data[0][j]=sigmoid(z.data[0][j]);
                zRowSum+=z.data[0][j];//���
            }
            for(j=0;j<z.col;j++)
            {
                z.data[0][j]/=zRowSum;//��һ��
                if(iter%1000==0)
                    cout<<z.data[0][j] <<" s ";
            }
            if(iter%1000==0)
                cout<<endl;
            for(j=0;j<y.col;j++)
            {
                z.data[0][j]=y.data[i][j]-z.data[0][j];
            }
            xOneRowT = xOneRow.transposeMatrix();
            grad = xOneRowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
            for(k=0; k<grad.row;k++)
            {
                for(j=0;j<grad.col; j++)
                {
                    grad.data[k][j]*= alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
                }
            }
            weights = weights + grad; ///����
        }
    }
    //��֤�㷨����ȷ��
    /**
    ��֤�㷨����ȷ��
    **/
    Matrix test=x * weights;
    cout<<"test"<<endl;
    for(i=0; i<y.row; i++)
    {
        if(test.data[i][0]>test.data[i][1])
            cout<<0-y.data[i][1]<<"  ";
        else
            cout<<1-y.data[i][1]<<"  ";
        cout<<endl;
    }
}

int SoftMaxReg(const char *file, const string &model, const double &alpha)
{
    Matrix x;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    x.LoadData(file);
    Matrix y;
    y=x.getOneCol(x.col-1);
    y=one_hot(y,2);
    x.deleteOneCol(x.col-1);

    if(model=="gradAscent")
        gradAscent_SoftMax(x,y);
    if(model=="stoGradAscent")
        stoGradAscent_SoftMax(x,y);

    return 0;
}
