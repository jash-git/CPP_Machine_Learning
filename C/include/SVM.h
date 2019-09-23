#include "MatrixOpe.h"
#define MAX 1000000
#define MIN -100000
//SMO�����ṹ��
struct OS
{
    Matrix x;
    Matrix y;
    double C;
    double soft;
    int m;
    Matrix alphas;
    double b;
    Matrix eCache;
    Matrix kernel;
    bool k;
};
//�˺����Ľṹ��
struct kTup
{
    int type;//0,1
    double arg;//�˺�������
};
class SMOP
{
    //�ǳ�ֵ��ע�����svm��ѵ�������������У���ÿһ����һ�����������Ե���w��������
public:
    OS os;
public:
    /**
    ���ݲ����������ɲ�ͬ�ĺ˺���
    */
    Matrix kernelTran(Matrix x,Matrix xOneRow,kTup ktup)
    {
        Matrix K(x.row,1,0,"ss");
        Matrix xOneRowT = xOneRow.transposeMatrix();
        if(ktup.type==1)
        {
            K = K.multsMatrix(x,xOneRowT);
        }
        if(ktup.type==2)
        {
            //��˹��
        }
        return K.transposeMatrix();
    }
    /**
    �ṹ��OS�ĳ�ʼ�������ڱ�������SMO�㷨����Ҫ�õ��Ĳ���
    */
    int initOs(Matrix x,Matrix y,double C,double soft,kTup ktup)
    {
        //os.x.initMatrix(x.col,x.row,0,"ss");
        os.x = x;
        //os.y.initMatrix(y.col,y.row,0,"ss");
        os.y = y;
        os.eCache.initMatrix(x.row,2,0,"ss");
        os.alphas.initMatrix(x.row,1,0,"ss");

        os.b=0;
        os.C=C;
        os.m=x.row;
        os.kernel.initMatrix(os.m,os.m,0,"s");
        os.soft=soft;
        if(ktup.type!=0)
        {
            int i=0,j=0;
            Matrix xOneRow(1,os.x.col,0,"ss");
            Matrix kOneCol(os.m,1,0,"ss");
            for(i=0; i<os.m; i++)
            {
                xOneRow=x.getOneRow(i);
                kOneCol=kernelTran(x,xOneRow,ktup);
                for(j=0; j<os.m; j++)
                    os.kernel.data[i][j]=kOneCol.data[0][j];
            }
            os.k=true;
            return 0;
        }
        os.k=false;
        return 0;
    }
    /**
    �����޼�������
    */
    double clipAlpha(double alpha,double L,double H)
    {
        if(alpha>H)
            return H;
        if(alpha<L)
            return L;
        return alpha;
    }
    /**
    ���ֵ�ļ���Ҳ�Ǹ�����ѧ���ʽ��ʵ�ֵ�

    **/
    double calcEk(int k)
    {
        Matrix ay(os.x.row,1,0,"ss");
        size_t i;
        for(i=0; i<ay.row; i++)
        {
            ay.data[i][0]=os.alphas.data[i][0]*os.y.data[i][0];
        }
        Matrix ayT =ay.transposeMatrix();
        Matrix f(1,1,0,"ss");
        if(!os.k)
        {
            Matrix ayTx =ayTx.multsMatrix(ayT,os.x);
            Matrix xOneRow = os.x.getOneRow(k);
            Matrix xOneRowT = xOneRow.transposeMatrix();
            f = f.multsMatrix(ayTx,xOneRowT);//ֵ����ȶ
        }
        else
        {
            Matrix kOneCol = os.kernel.getOneCol(k);
            f = f.multsMatrix(ayT,kOneCol);//ֵ����ȶ
        }
        double fXK=f.data[0][0]+os.b-os.y.data[k][0];
        return fXK;
    }
    ///����������
    void updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//�������ֵ
        os.eCache.data[k][0]=1;
        os.eCache.data[k][1]=Ek;
    }
    /**
    ���ڵڶ���alpha��ѡ�����Ei-Ej�Ĵ�С���к�����ͬʱ��������һ�����������治���E�����ټ�����
    ѡ������ʵڶ���alpha2�����ڵ�һ�ν������������ֵE�ľ����Ȼû��һ����Ч�ģ����Բ����������
    ���ѡ�������ʵ��������ѡ�񣬾���û��ʵ�֣����ǲ��õȼ۷�ʽʵ�֣�����Ҫע������Խ������
    **/
    int selectJ(int i,double Ei)
    {
        int maxK=-1;
        double Ek;
        double Ej;
        double deltaE;
        double maxDeltaE=0;
        size_t k;
        for(k=0; k<os.alphas.col; k++)
        {
            if(os.eCache.data[k][0]==1&&k!=i)
            {
                Ek=calcEk(k);
                deltaE=fabs(Ei-Ek);
                if(deltaE>maxDeltaE)
                {
                    maxK=k;
                    maxDeltaE=deltaE;
                    Ej=Ek;
                }
            }
            //���ѡ��һ��j, ������ֻ�е�һ��ʱ���е�ek��û�������������ѡһ������
            if(maxK==-1)
            {
                maxK=(i*2+1)%100;
                Ej=calcEk(maxK);
            }
        }
        return maxK;
    }

    /**
    �ڲ�ѭ��ʵ������alpha���Ż��������ѭ�������ṩalpha1�Ŀ�ѡ���ϣ���������������֧����������
    ���������ϵ�ÿһ��alpha����������ж��Ƿ�Υ��KKT���������Υ����ѡΪ��һ��alpha1��
    ͬ������selctJ����������ʽѡ��ʹ��alpha1�ı�����alpha2��
    ѡ�������ij֮�󣬻��ø��ݵ�ǰalpha1��alpha2��ȷ���Ż��������ޡ�
    Ȼ��ͨ��������ʽ��������alpha1��alpha2�������ʱ���ж��Ƿ���ú˷�����
    ���Լ��������������޼���������ȷ���Ż����alpha1��alpha2��ͬʱ�Ե�ǰ�Ż�������bҲ�����޸�
    ���ǵ�ÿһ��ѡ��ڶ���alpha2ʱ������Ҫ�Ƚ�һ�����ֵ����������ֵ��ÿ���Ż������ж�ֻ�ж�Ӧ�޸�alpha�����ֵ�б仯
    ���������ǲ���ģ�������һ��������������Ч�����ֵ��
    */
    int innerL(int i)
    {
        double Ei=0;
        double Ej=0;
        int j;
        double alphaIOld;
        double alphaJOld;
        double L;
        double H;
        double eta;
        int n;
        double temp[3];//ii��jj��ij
        double b1,b2;
        if(os.y.data[i][0]*(Ei-os.y.data[i][0]*os.soft)<0&&os.alphas.data[i][0]<os.C||os.y.data[i][0]*(Ei-os.y.data[i][0]*os.soft)>0&&os.alphas.data[i][0]>0)
        {
            Ei=calcEk(i);
            j=selectJ(i,Ei);
            Ej=calcEk(j);
            alphaIOld=os.alphas.data[i][0];
            alphaJOld=os.alphas.data[j][0];
            if(os.y.data[i][0]!=os.y.data[j][0])
            {
                L=max<double>(0.0,os.alphas.data[j][0]-os.alphas.data[i][0]);
                H=min(os.C,os.alphas.data[j][0]-os.alphas.data[i][0]+os.C);
            }
            else
            {
                L=max<double>(0.0,os.alphas.data[j][0]+os.alphas.data[i][0]-os.C);
                H=min<double>(os.C,os.alphas.data[j][0]+os.alphas.data[i][0]);
            }
            if(L==H)
            {
                cout<<"l=h------------------"<<endl;
                return 0;
            }
            if(!os.k)
            {
                temp[0]=0;
                temp[1]=0;
                temp[2]=0;
                for(n=0; n<os.x.row; n++)
                {
                    temp[0]+=pow(os.x.data[i][n],2);
                    temp[1]+=pow(os.x.data[j][n],2);
                    temp[2]+=os.x.data[i][n]*os.x.data[j][n];
                }
            }
            else
            {
                temp[0]=os.kernel.data[i][i];
                temp[1]=os.kernel.data[j][j];
                temp[2]=os.kernel.data[i][j];
                eta=temp[0]+temp[1]-2*temp[2];
            }
            eta=temp[0]+temp[1]-2*temp[2];
            if(eta<0)
            {
                cout<<"eta<0-----------------"<<endl;
                return 0;
            }
            os.alphas.data[j][0]+=os.y.data[j][0]*(Ei-Ej)/eta;
            os.alphas.data[j][0]=clipAlpha(os.alphas.data[j][0],L,H);//��alpha���м���
            updataEk(j);//�������ֵ
            if(fabs(os.alphas.data[j][0]-alphaJOld)<0.00000000001)
            {
                cout<<"j not moving enough---------------"<<endl;
                return 0;
            }
            os.alphas.data[i][0]+=os.y.data[i][0]*os.y.data[j][0]*(alphaJOld-os.alphas.data[j][0]);
            updataEk(i);//�������ֵ

            b1=os.b-Ei-os.y.data[i][0]*(os.alphas.data[i][0]-alphaIOld)*temp[0]-os.alphas.data[j][0]*(os.alphas.data[j][0]-alphaJOld)*temp[2];
            b2=os.b-Ej-os.y.data[i][0]*(os.alphas.data[i][0]-alphaIOld)*temp[2]-os.alphas.data[j][0]*(os.alphas.data[j][0]-alphaJOld)*temp[1];
            if(0<os.alphas.data[i][0]&&os.C>os.alphas.data[i][0])
                os.b=b1;
            else if(0<os.alphas.data[j][0]&&os.C>os.alphas.data[j][0])
                os.b=b2;
            else
                os.b=(b1+b2)/2;
            return 1;
        }
        cout<<"kkt--------------------------"<<endl;
        return 0;
    }

    /**

    SMO�㷨����ں���������Ҫ�����ǳ�ʼ��SMO���еĲ������ṹ��OS
    ȷ������������־����������������֧��������ѭ����ѡ�����alpha1������innerȷ��alpha1��
    ���ѭ��ÿһ���ǵ����Ż�һ��alpha,�ڲ�ѭ����������ʵ�ֵ����Ż�һ��alpha
    ��󣬶�smo�㷨��ʵ�ֽ��м�飬ͨ��alpha����w��b������ѵ�������ϱȽ���Ԥ��ֵ��ʵ��ֵ�Ĳ��졣
    �ð취ֻ�ܼ��SMO�㷨ʵ�ֵ���ȷ�ԣ����ܼ��SVM�����ܡ�
    */

    int smoP(Matrix x,Matrix y,double C,double soft,int maxIter,kTup ktup)
    {
        int iter=0;
        int i;
        initOs(x,y,C,soft,ktup);///��ʼ��OS�ṹ�壬OS�ṹ���ж�����SMOP�㷨��Ҫ�Ĵ󲿷ֲ���
        bool entireSet=true;//��־�����жϵ�ǰ�������������alpha���������0-C֮��Ĳ���alpha��������ʵ��һ��alpha������ʽѡ�񣬼�ѡ������֧�������ϵĵ�
        int alphaPairsChanged=0;//�ò����ж���ȷ����һ������֮���Ƿ���ѡ�������Ҫ��ĵڶ�alpha������ֵΪ1��ʾ�ܣ�0Ϊ����
        for(iter=0; iter<maxIter&&(alphaPairsChanged>0||entireSet); iter++)
        {
            //ѭ��������־Ϊ���������ѵ�Ԥ��ֵ�������ǲ����ټ����Ż����������е�֧�������ϵĵ㶼�Ҳ����ڶ���alpha�Ե�һ��alpha�����Ż��������ٱ������еĵ�Ѱ�ҿ��Ż��Ĳ����ԣ�
            //�����Ҳ������ٴα���֧�������ϵĵ㣬��α�ȻҲ���Ҳ������Ž�������
            alphaPairsChanged=0;
            if(entireSet)
            {
                for(i=0; i<os.m; i++)
                {
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<"  entireSet="<<entireSet<<endl;
                }
                iter++;
            }
            else
            {
                for(i=0; i<os.m; i++)
                {
                    if(os.alphas.data[i][0]>0&&os.alphas.data[i][0]<os.C)//ֻѡ��֧�������ϵĵ�
                        continue;
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<alphaPairsChanged<<"  entireSet="<<entireSet<<endl;
                }
                iter++;
            }
            if(entireSet)
                entireSet=false;
            else if(alphaPairsChanged==0)
                entireSet=true;
        }
        ///��SMO�㷨ʵ�ֵ���ȷ�Խ�����֤�����Ԥ��ֵ��ʵ��ֵ�Ĳ��ȫΪ0��ʾ��ѵ��������Ԥ��ȫ��
        Matrix ay(os.x.row,1,0,"ss");
        for(i=0; i<os.m; i++)
        {
            ay.data[i][0]=os.alphas.data[i][0]*os.y.data[i][0];
        }

        Matrix xT = x.transposeMatrix();

        Matrix w = w.multsMatrix(xT,ay);

        Matrix label = label.multsMatrix(os.x,w);
        cout<<os.b<<"  ";
        for(i=0; i<os.x.col; i++)
        {
            cout<<w.data[i][0]<<"  ";
        }
        cout<<endl;
        cout<<"-----------"<<endl;
        ///��֤ѵ���������ݣ���֤SVMʵ�ֵ���ȷ��
        for(i=0; i<os.m; i++)
        {
            if((label.data[i][0]+os.b)>0)
                cout<<1-os.y.data[i][0]<<"  ";
            else
                cout<<0-os.y.data[i][0]<<"  ";
        }
        return 0;
    }

};
int SVM();


