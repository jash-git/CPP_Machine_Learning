/**

HMMģ���������������⣬���ʼ������⣬ѧϰ�������⣬Ԥ�����⡣
���������������HMMģ���е�

1 ǰ������㷨

2 Baum-Welch�㷨

3 Veiterbi�㷨

����ĳ��������������㷨��ʵ�֣����ǵ����ڷִʣ�������Ƴɶ�ȡ�ļ�����ʽ
��ʵ�������ķִ��㷨�е�ѧϰ�������Ⲣ������Baum-Welch�㷨�����ǲ��üලѧϰ�㷨�еļ�����Ȼ���ƣ�
����Baum-Welch�㷨�ж�ȡ���ļ�ֻ��һ�����ٴʵ��ļ�����EM�㷨���ѧϰ������
������ѧϰ����������д������ѧϰ�㷨��һBaum-Welch�㷨���� ������Ȼ����
������ͳ��ѧϰ����һ���е����ӽ�����֤Veiterbi�㷨����ȷ�ԡ�
���������һ���ļ���ʵ�ּ�����Ȼ��HMMģ�͵����ķִ�

����ֻ����һ������

**/

#include "MatrixOpe.h"
#define VEC_LEN 10000
#define STATE 3
#define T 1000
#define double_min -1
using namespace std;

struct HMM_Pra
{
    string dic[VEC_LEN];
    double arg[VEC_LEN];//��Ƶ
    double a[STATE][STATE];//����A
    double b[STATE][VEC_LEN];//����B
    double pi[STATE];//����pi
    int len;
};
double alpha[T][STATE];
double bata[T][STATE];
double epsion[T][STATE][STATE];
double Epsion[STATE][STATE];
double Gam[STATE];
double Gamvk[STATE][T];
double sum[STATE];
double sumall=0;
HMM_Pra hmm_pra;

void createOset(const DataStr &data)
{
    ofstream ofile_arg;
    ofstream ofile;
    ofile.open("data\\hmm_dic.txt");
    int i,j,k,vl;
    int dic_len=0;
    int length=0;
    for(i=0; i<data.size(); i++)
    {
        for(j=0; j<data[i].size(); j++)
        {
            length++;
            for(vl=0; vl<dic_len; vl++)
            {
                if(!data[i][j].compare(hmm_pra.dic[vl]))
                {
                    hmm_pra.arg[vl]++;//�Դ�Ƶ����ͳ�ƣ���Ϊ���ʲ���
                    break;
                }
            }
            if(vl==dic_len)
            {
                hmm_pra.dic[vl]=data[i][j];//���ֵ������չ
                hmm_pra.arg[vl]=1;
                ofile<<data[i][j]<<"  ";
                dic_len++;
            }
        }
    }
    ofile.close();
    hmm_pra.len=dic_len;
    ofile_arg.open("data\\hmm_arg.txt");
    for(i=0; i<hmm_pra.len; i++)
        ofile_arg<<hmm_pra.arg[i]<<"  ";
    ofile_arg.close();
    cout<<"vec_len="<<dic_len<<endl;
}
int getPos_O(string str)
{
    int i=0,j;
    for(i=0; i<hmm_pra.len; i++)
    {
        if(!str.compare(hmm_pra.dic[i]))
            return i;
    }
    cout<<"״̬�ֵ��в����ڸ�״̬"<<endl;
    return -1;
}

/**
ͳ��ѧϰ����p186�������VTB�㷨��ȷ��
��Ntest�ļ��е��ֵ䳤�ȿ����۲����п���ֵ�������Ŀ����۲�����
����״̬�ڲ���STATE���ã����ֹ��ʷִ���STATE=4�����Ա���ʱ�޸�ΪSTATE=3
**/

void init_test()
{
    double A[STATE][STATE]= {0.5,0.2,0.3,
                     0.3,0.5,0.2,
                     0.2,0.3,0.5
                    };
    double B[STATE][2]= {0.5,0.5,
                     0.4,0.6,
                     0.7,0.3
                    };
    double pi[STATE]= {0.2,0.4,0.4};
    int i,j;

    for(i=0; i<STATE; i++)
    {
        for(j=0; j<STATE; j++)
        {
            hmm_pra.a[i][j]=A[i][j];
        }
    }
    for(i=0; i<STATE; i++)
    {
        for(j=0; j<hmm_pra.len; j++)
        {
            hmm_pra.b[i][j]=B[i][j];
        }
    }
    for(i=0; i<STATE; i++)
    {
        hmm_pra.pi[i]=pi[i];
    }
}

void forwardBack(const DataStr &data)
{
    //ǰ���㷨
    int s,t,i,j,k;
    int pos;
    sumall=0;
    for(i=0; i<STATE; i++)
    {
        pos=getPos_O(data[0][0]);
        alpha[0][i]=hmm_pra.pi[i]*hmm_pra.b[i][pos];
    }
    for(t=1; t<data[0].size(); t++)
    {
        for(i=0; i<STATE; i++)
        {
            sum[i]=0;
            for(j=0; j<STATE; j++)
            {
                sum[i]+=alpha[t-1][j]*hmm_pra.a[j][i];
            }
            pos=getPos_O(data[0][t]);
            alpha[t][i]=sum[i]*hmm_pra.b[i][pos];
        }
    }
    for(i=0; i<STATE; i++)
    {
        sumall+=alpha[data[0].size()-1][i];
    }
    cout<<"FORWARD��"<<"sum="<<sumall<<endl;

    //�����㷨
    for(i=0; i<STATE; i++)
    {
        bata[data[0].size()-1][i]=1;
    }
    for(t=data[0].size()-2; t>=0; t--)
    {
        for(i=0; i<STATE; i++)
        {
            sum[i]=0;
            pos=getPos_O(data[0][t+1]);
            for(j=0; j<STATE; j++)
            {
                sum[i]+=hmm_pra.a[i][j]*hmm_pra.b[j][pos]*bata[t+1][j];
            }
            bata[t][i]=sum[i];
        }
    }
    sumall=0;
    pos=getPos_O(data[0][0]);
    for(i=0; i<STATE; i++)
    {
        sumall+=bata[0][i]*hmm_pra.pi[i]*hmm_pra.b[i][pos];
    }
    cout<<"BACK��"<<"sum="<<sumall<<endl;
}

void Baum_Weach(const DataStr &data)
{
    //Baum-Welch�㷨����������
    //һЩ����������ֵ�ļ���
    int s,t,i,j,k,iter;
    int pos;
    for(iter=0; iter<10; iter++)
    {
        forwardBack(data);
        /**
        ����A
        **/
        cout<<"A-----------------"<<endl;
        for(i=0; i<STATE; i++)
        {
            for(j=0; j<STATE; j++)
            {
                Epsion[i][j]=0;
            }
        }

        for(t=0; t<data[0].size()-1; t++)
        {
            pos=getPos_O(data[0][t+1]);
            for(i=0; i<STATE; i++)
            {
                for(j=0; j<STATE; j++)
                {
                    epsion[t][i][j]=alpha[t][i]*hmm_pra.a[i][j]*hmm_pra.b[j][pos]*bata[t+1][j];
                    epsion[t][i][j]/=sumall;
                    Epsion[i][j]+=epsion[t][i][j];
                }
            }
        }
        for(i=0; i<STATE; i++)
        {
            Gam[i]=0;
        }
        for(t=0; t<data[0].size()-1; t++)
        {
            for(i=0; i<STATE; i++)
            {
                Gam[i]+=(alpha[t][i]*bata[t][i])/sumall;
            }
        }

        for(i=0; i<STATE; i++)
        {
            for(j=0; j<STATE; j++)
            {
                hmm_pra.a[i][j]=Epsion[i][j]/Gam[i];
                cout<<hmm_pra.a[i][j]<<"  ";
            }
            cout<<endl;
        }
        /**
        ����B
        **/
        cout<<"B----------------"<<endl;
        for(i=0; i<STATE; i++)
        {
            for(t=0; t<data[0].size(); t++)
            {
                Gamvk[i][t]=0;
            }
        }
        for(k=0; k<hmm_pra.len; k++)
        {
            for(t=0; t<data[0].size(); t++)
            {
                pos=getPos_O(data[0][t]);
                if(pos==k)
                {
                    for(i=0; i<STATE; i++)
                    {
                        Gamvk[i][k]+=(alpha[t][i]*bata[t][i])/sumall;//�����t+1��ʱ��ʾvk��Ƶ��
                    }
                }
            }
        }
        for(i=0; i<STATE; i++)
        {
            Gam[i]=0;
        }
        for(t=0; t<data[0].size(); t++)
        {
            for(i=0; i<STATE; i++)
            {
                Gam[i]+=(alpha[t][i]*bata[t][i])/sumall;
            }
        }
        for(i=0; i<STATE; i++)
        {
            for(k=0; k<hmm_pra.len; k++)
            {
                hmm_pra.b[i][k]=Gamvk[i][k]/Gam[i];
                cout<<hmm_pra.b[i][k]<<"  ";
            }
            cout<<endl;
        }

        /**
        ����pi
        **/
        cout<<"pi----------------"<<endl;
        for(i=0; i<STATE; i++)
        {
            hmm_pra.pi[i]=(alpha[0][i]*bata[0][i])/sumall;
            cout<<hmm_pra.pi[i]<<"  ";
        }
        cout<<endl;
    }
}

void Viterbi_O(const DataStr &data)
{
    int t,i,j,k;
    int pos;
    double deta[VEC_LEN][STATE];
    int fai[VEC_LEN][STATE];
    double max_deta;
    double max_fai;
    int max_i;
    for(i=0; i<STATE; i++)
    {
        pos=getPos_O(data[0][0]);
        deta[0][i]=hmm_pra.pi[i]*hmm_pra.b[i][pos];
        fai[0][i]=0;
    }
    for(t=1; t<data[0].size(); t++)
    {
        for(i=0; i<STATE; i++)
        {
            max_deta=double_min;
            max_fai=double_min;
            for(j=0; j<STATE; j++)
            {
                pos=getPos_O(data[0][t]);
                if(deta[t-1][j]*hmm_pra.a[j][i]*hmm_pra.b[i][pos]>max_deta)
                {
                    max_deta=deta[t-1][j]*hmm_pra.a[j][i]*hmm_pra.b[i][pos];
                }
                if(deta[t-1][j]*hmm_pra.a[j][i]>max_fai)
                {
                    max_fai=deta[t-1][j]*hmm_pra.a[j][i];
                    max_i=j;
                }
            }
            deta[t][i]=max_deta;
            fai[t][i]=max_i;
        }
    }
    max_deta=double_min;
    for(i=0; i<STATE; i++)
    {
        if(deta[data[0].size()-1][i]>max_deta)
        {
            max_deta=deta[data[0].size()-1][i];
            max_i=i;
        }
        cout<<"P*="<<deta[data[0].size()-1][i]<<endl;
    }
    cout<<max_i;
    for(t=data[0].size()-2; t>=0; t--)
    {
        cout<<fai[t+1][max_i];
        max_deta=double_min;
        for(i=0; i<STATE; i++)
        {
            if(deta[t][i]>max_deta)
            {
                max_deta=deta[data[0].size()-1][i];
                max_i=i;
            }
        }
    }
}

int HMM()
{
    DataStr data;
    LoadDataStr(data,"data\\Ntest.txt");//����ѵ��������Ϊ���Լ���ֻ������һ������
    createOset(data);//�����ʵ�
    init_test();
    cout<<"-------------forwardBack-----------------"<<endl;
    forwardBack(data);
    cout<<"-------------Baum_Weach-----------------"<<endl;
    Baum_Weach(data);
    cout<<"-------------Viterbi-----------------"<<endl;
    init_test();
    Viterbi_O(data);
    return 0;
}


