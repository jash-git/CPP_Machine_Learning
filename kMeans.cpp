/**
Kmeans�㷨�����ľ��Ǿ۳ɿ���ĳ��֣�ֵ�÷���һ��kmeansʲô����»���ֿ��࣬
�ó�����δ�Դ�����������϶����ǻ�������������
**/
#include "MatrixOpe.h"
#define MAX 1000000
#define MIN -100000
#define random(x) (rand()%x)
#define MAXK 10

/**
Kmeans�����������ݽṹ
*/
struct CenAndDis
{
    Matrix cen;//��������
    Matrix dis;//ÿһ����������������ĵľ��룬��Ϊһ��m�е�������
};

///����һ��������ĳһ���������ĵľ���
double distances(Matrix xOne,Matrix kCen)
{
    int i,j;
    double dis;
    for(i=0; i<xOne.col; i++)
    {
        dis=pow((xOne.data[0][i]-kCen.data[0][i]),2);
    }
    return dis;
}

/**
�����ʼ��һ���������ģ���ʼ���ķ�ʽ�����֣�һ���������������ѡһ����Ϊ�������ģ�
һ�����������ռ��ڳ�ʼ��һ���������ģ����Բ��Ǿ���ĳ�������������ĵĳ�ʼ���ǳ��ؼ�

������õ��Ǵ�������ѡ����Ϊ�������ģ���������ѡȡ��������ͬ��Ҳ����־۳ɿ�������󣬶Դ˽���취��

**/
Matrix randCent(Matrix x,int kNum)
{
    int i,j,k;
    Matrix initmeans;
    initmeans.initMatrix(kNum,x.col,0,"ss");

    double min[2];
    double max[2];

    /**
    �������ռ��������ʼ��k��������
    **/
    for(j=0; j<x.col; j++)
    {
        min[j]=MAX;
        max[j]=MIN;
        for(i=0; i<x.row; i++)
        {
            if(x.data[i][j]<min[j])
            {
                min[j]=x.data[i][j];
            }
            if(x.data[i][j]>max[j])
            {
                max[j]=x.data[i][j];
            }
        }
    }
    /**
    �����������ѡ��k��������Ϊ������
    */
    for(k=0; k<kNum; k++)
    {
        for(j=0; j<x.col; j++)
        {
            //�ı��ֵ��ѡȡ���õ��ľ������Ļ᲻һ�£��ɼ���ֵ��ѡȡ�Ĺؼ���
            initmeans.data[k][j]=x.data[k][j];//min[j]+(max[j]-min[j])*(random(10)/10.0);
            cout<<initmeans.data[k][j]<<"  ";
        }
        cout<<endl;
    }
    return initmeans;
}
CenAndDis kMeans(Matrix &x,const int &kNum, const int &Iter)
{
    int i,j,k,it;
    double dis[MAXK];///��¼һ��������k�������ĵľ��룬���Բ��þ���̬������ã�����Ĭ�����MAXK��������

    Matrix kmeans;//��������
    kmeans=randCent(x,kNum);//�����ʼ��k��������

    Matrix xOne;//�洢һ������
    Matrix kOne;//�洢һ��������������
    Matrix kLabel;//�洢ÿ�����������
    kLabel.initMatrix(x.row,1,0,"ss");

    double minDis=MAX;//��С�ľ���

    for(it=0; it<Iter; it++)
    {
        //����k���������Ļ������ǩ
        for(i=0; i<x.row; i++)
        {
            xOne = x.getOneRow(i);
            minDis=MAX;
            for(k=0; k<kNum; k++)
            {
                kOne = kmeans.getOneRow(k);
                dis[k]=distances(xOne,kOne);
                if(dis[k]<minDis)
                {
                    kLabel.data[i][0]=k;
                    minDis=dis[k];
                }
            }
        }
        //kmeans���㣬�����������Ĳ���+=����ʽ����Ҫ���㣬���������kmeansͬ���ǲ���+=����ʽ����Ϊ�ұ���һ�����kmeans
        for(k=0; k<kNum; k++)
        {
            for(j=0; j<x.col; j++)
            {
                kmeans.data[k][j]=0;
            }
        }
        //����kmeans
        for(i=0; i<x.row; i++)
        {
            k=kLabel.data[i][0];
            for(j=0; j<x.col; j++)
            {
                kmeans.data[k][j]=(kmeans.data[k][j]*(i)+x.data[i][j])/(i+1);//�����ۼӷ�ʽ���ֵ���÷������Ӽ�����
                //��ȻҲ���԰�һ�����������ȡ�����������ȡ��ֵ������û����������������һ����һ��
            }
        }
        //�����ǰ��EM��ľ�������
        cout<<"--------------------"<<endl;
        for(k=0; k<kNum; k++)
        {
            for(j=0; j<x.col; j++)
            {
                cout<<kmeans.data[k][j]<<"  ";
            }
            cout<<endl;
        }
    }
    /**
    �����������浽�ṹ����
    */
    CenAndDis cendis;
    cendis.cen.initMatrix(kNum,x.col,0,"b");
    cendis.dis.initMatrix(x.row,1,0,"ss");
    cendis.cen = kmeans;

    ///���������������������ĵľ��뵽�ṹ����
    for(i=0; i<x.row; i++)
    {
        k=kLabel.data[i][0];
        xOne=x.getOneRow(i);
        kOne=kmeans.getOneRow(k);
        cendis.dis.data[i][0]=distances(xOne,kOne);
    }
    //
    double sum=0;
    for(i=0; i<x.row; i++)
    {
        sum+=cendis.dis.data[i][0];
    }
    cout<<"err="<<sum<<endl;
    return cendis;
}

Matrix subMatrix(const Matrix &x, const Matrix &clusterAssment,const int &label)
{
    int i=0,j=0,k=0;
    Matrix submatrix;
    submatrix = x;
    for(i=0; i<x.row; i++)
    {
        if( int(clusterAssment.data[i][0])==label)
        {
            for(j=0; j<x.col; j++)
            {
                submatrix.data[k][j]=x.data[i][j];
            }
            k++;
        }
    }
    submatrix.row=k;
    return submatrix;
}
/***
���־����˼����ÿһ�ζ�����һ�����͵Ķ�����࣬����ѡ����Ǹ�������ж��ֵ����۱�׼��SSE��
��������һ��ʹ��SSE��С�ͷ�����һ�࣬SSE����Сƽ��������ʹ�þ������ľ�����Сƽ���������С
�Ͷ���һ����ж��֣�ֱ�����ټ���SSE��������һ��Ϊ���࣬���ߵ���Ԥ��ľ�ΪK��

**/
int biKmeans(Matrix &x,const int &kNum,const int &Iter)
{
    int i,j,k,d;
    Matrix kmeans;
    kmeans.initMatrix(kNum,x.col,0,"ss");///��ʼ����Ϊһ�࣬

    Matrix xOne;//һ������
    Matrix kOne;//һ����������
    Matrix clusterAssment;///����ĵ�һ�б����䵽������𣬵ڶ��б����������������ľ���
    clusterAssment.initMatrix(x.row,2,0,"ss");

    CenAndDis cenanddis;///������
    cenanddis.cen.initMatrix(kNum,x.col,0,"ss");
    cenanddis.dis.initMatrix(x.col,1,0,"ss");

    CenAndDis bestCenanddis;///��¼��ǰ��õľ�����
    bestCenanddis.cen.initMatrix(kNum,x.col,0,"ss");
    bestCenanddis.dis.initMatrix(x.row,1,0,"ss");

    for(i=0; i<x.row; i++)
    {
        for(j=0; j<x.col; j++)
        {
            kmeans.data[0][j]=(kmeans.data[0][j]*i+x.data[i][j])/(i+1);
        }
        clusterAssment.data[i][0]=0;///��ʼ����Ϊһ�࣬������Ϊ֮ǰ��ʼ��Ϊһ���������
    }
    for(i=0; i<x.row; i++)
    {
        xOne=x.getOneRow(i);
        clusterAssment.data[i][1]=distances(xOne,kmeans);///��ʼ��Ϊһ���������������������ľ���
    }

    Matrix submatrix;///���Ѻ���Ӽ�����
    submatrix = x;

    double lowestSSE=MAX;///��¼��ǰ��������SSEֵ
    double sseSplit=0;///��¼���Ѻ��Ӽ���SSEֵ
    double sseNosplit=0;///��¼û�з��ѵ��Լ���SSEֵ
    int bestCentToSplit;///��¼Ԥ�����Ӽ������
    double dis[2];///��¼���Ѻ������Ӽ���SSEֵ
    for(d=1; d<kNum; d++)
    {
        lowestSSE=MAX;//��������ʼ�������óɵ�ǰ�������µ��ܾ��룬��ô���ܱ�֤һ���õ�kNum��������
        for(k=0; k<d; k++)
        {
            submatrix=subMatrix(x,clusterAssment,k);///���ڱ�������k����Ӽ���������kmeans���Խ��л���
            cenanddis=kMeans(submatrix,2,10);///������kmeans���ж���
            sseSplit=0;
            sseNosplit=0;
            /**
            ��������SSEֵ��
            */
            for(i=0; i<submatrix.row; i++)
            {
                sseSplit+=cenanddis.dis.data[i][1];
            }
            /**
            δ���ѵ�����SSEֵ��
            */
            for(i=0; i<x.row; i++)
            {
                if(clusterAssment.data[i][0]!=k)
                {
                    sseNosplit+=clusterAssment.data[i][1];
                }
            }
            ///����С��SSE���бȽϣ�ѡ�����ѵķ�����
            if(sseSplit+sseNosplit<lowestSSE)//�������k���ؽ��ж����ָ��ã����¼������������lowsetSSE
            {
                bestCentToSplit=k;
                bestCenanddis=cenanddis;
                lowestSSE=sseSplit+sseNosplit;
            }
        }
        ///���ȷ��ѡ����õĶ����ִ�֮�����濪ʼ��x�����Ͻ��и��¾������ĺ;��룬clusterAssment����
        for(i=0; i<x.row; i++)
        {
            if(clusterAssment.data[i][0]==bestCentToSplit)
            {
                xOne=x.getOneRow(i);
                for(k=0; k<2; k++)
                {
                    kOne=bestCenanddis.cen.getOneRow(k);
                    dis[k]=distances(kOne,xOne);
                }
                if(dis[0]<dis[1])
                {
                    clusterAssment.data[i][0]=bestCentToSplit;///���Ѻ�������࣬��һ���������ǵ�ǰ���ֵ
                    clusterAssment.data[i][1]=dis[0];
                }
                else
                {
                    clusterAssment.data[i][0]=bestCentToSplit+1;///���Ѻ�������࣬�ڶ������������һ���µ����ֵ��һֱ���ϼ�1
                    clusterAssment.data[i][1]=dis[1];
                }
            }
        }
        ///�������յ�SSEֵ
        double sum=0;
        for(i=0; i<x.row; i++)
        {
            sum+=clusterAssment.data[i][1];
        }
        cout<<"err==="<<sum<<endl;
    }
}

int kmeans()
{
    Matrix x;
    x.LoadData("data\\kmeans.txt");
    kMeans(x,3,10);
    biKmeans(x,3,10);
}

