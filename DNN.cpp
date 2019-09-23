#include "MatrixOpe.h"
#define MAX_LAYER_N 1000
/**
�������W,b����Z,A��Ϊ�����в��
**/
struct parameters
{
    Matrix W;
    Matrix b;
    Matrix Z;
    Matrix A;
    Matrix D;
    parameters *next;
    parameters *pre;
};

/***
���Ϻ����ж�ӦĿ�꺯������Ӧ������ƫ��
**/
struct grad
{
    Matrix grad_W;
    Matrix grad_b;
    Matrix grad_Z;
    Matrix grad_A;
    Matrix V_dw;
    Matrix V_db;
    Matrix S_dw;
    Matrix S_db;
    Matrix V_dw_corrected;
    Matrix V_db_corrected;
    Matrix S_dw_corrected;
    Matrix S_db_corrected;
    grad *next;
    grad *pre;
};

/**
�����糬����
**/
struct sup_parameters
{
    int layer_dims;//���������
    int layer_n[MAX_LAYER_N];//ÿ����Ԫ����
    string layer_active[MAX_LAYER_N];//ÿ�㼤���
};

parameters par;// �����������ȫ�ֱ���

grad gra;//�����ݶ�ȫ�ֱ���

sup_parameters sup_par;//��������ܹ�����ȫ�ֱ���

/**
���в���������Ϊȫ�ֱ������ṹ�岻��Ҫ�ں���֮�䴫�ݣ�
���ݳ�������ʼ������
**/
int init_parameters(Matrix X,const char *initialization)
{
    int k=0,i=0,j=0;
    double radom;
    int L=sup_par.layer_dims;//�������
    parameters *p=&par;//�������ṹ���Ѷ��岢�����ڴ棬�ṹ���ھ���δ�����ڴ�
    grad *g=&gra;//�ݶȣ��ṹ���Ѷ��岢�����ڴ棬�ṹ���ھ���δ�����ڴ�
    /**
        �����ʼ��
    **/
    //p->A.initMatrix(sup_par.layer_n[k],X.col,0,"ss");

    //p->AT.initMatrix(X.col,X.row,0,"ss");
    for(k=0; k<L-1; k++)
    {
        p->A.initMatrix(sup_par.layer_n[k],X.col,0,"ss");
        //����dropout�������ʼ��һ�μ��ɣ����浱ʹ��dropoutʱ��D�Żḳֵ����ʹ���򲻸�ֵ����ʵ��ʹ�ó���С���������
        p->D.initMatrix(sup_par.layer_n[k],X.col,0,"ss");
        p->W.initMatrix(sup_par.layer_n[k+1],sup_par.layer_n[k],0,"ss");
        p->b.initMatrix(sup_par.layer_n[k+1],1,0,"ss");
        p->Z.initMatrix(sup_par.layer_n[k+1],X.col,0,"ss");

        for(i=0; i<p->W.row; i++)
        {
            for(j=0; j<p->W.col; j++)
            {
                if(initialization=="he")
                {
                    radom=(rand()%100)/100.0;
                    p->W.data[i][j]=radom * sqrt(2.0/sup_par.layer_n[k]);//һ�ֳ��õĲ�����ʼ��������������ʼ��Ҳ�м���
                }
                if(initialization=="random")
                {
                    radom=(rand()%100)/100.0;
                    p->W.data[i][j]=radom;//һ�ֳ��õĲ�����ʼ��������������ʼ��Ҳ�м���
                }
                if(initialization=="arxiv")
                {
                    radom=(rand()%100)/100.0;
                    p->W.data[i][j]=radom/sqrt(sup_par.layer_n[k]);//һ�ֳ��õĲ�����ʼ��������������ʼ��Ҳ�м���
                }
            }
        }
        p->next=new parameters();//��һ���������
        p->next->pre=p;
        p=p->next;

        g->grad_A.initMatrix(sup_par.layer_n[L-k-1],X.col,0,"ss");
        g->grad_Z.initMatrix(sup_par.layer_n[L-k-1],X.col,0,"ss");
        g->grad_W.initMatrix(sup_par.layer_n[L-k-1],sup_par.layer_n[L-k-2],0,"ss");
        g->grad_b.initMatrix(sup_par.layer_n[L-k-1],1,0,"ss");

        //����momentum ��adam�Ż������ڱ���ǰn�μ�Ȩƽ��ֵ
        g->V_dw.initMatrix(sup_par.layer_n[L-k-1],sup_par.layer_n[L-k-2],0,"ss");
        g->V_db.initMatrix(sup_par.layer_n[L-k-1],1,0,"ss");

        g->S_dw.initMatrix(sup_par.layer_n[L-k-1],sup_par.layer_n[L-k-2],0,"ss");
        g->S_db.initMatrix(sup_par.layer_n[L-k-1],1,0,"ss");

        //����������momentum ��adam
        g->V_dw_corrected.initMatrix(sup_par.layer_n[L-k-1],sup_par.layer_n[L-k-2],0,"ss");
        g->V_db_corrected.initMatrix(sup_par.layer_n[L-k-1],1,0,"ss");

        g->S_dw_corrected.initMatrix(sup_par.layer_n[L-k-1],sup_par.layer_n[L-k-2],0,"ss");
        g->S_db_corrected.initMatrix(sup_par.layer_n[L-k-1],1,0,"ss");

        g->pre=new grad();//��һ����������ݶ�
        g->pre->next=g;
        g=g->pre;
    }
    p->A.initMatrix(sup_par.layer_n[k],X.col,0,"ss");
    g->grad_A.initMatrix(sup_par.layer_n[L-k-1],X.col,0,"ss");

    return 0;
}

void line_forward(parameters *p,double keep_prob)
{
    int i=0,j=0;
    if(keep_prob!=1)
    {
       for(i=0; i<p->D.row; i++)
        {
            for(j=0; j<p->D.col; j++)
            {
                p->D.data[i][j]=(rand()%100)/100.0;

                if(p->D.data[i][j]<keep_prob)
                    p->D.data[i][j]=1.0/keep_prob; //�����Ѿ�������keep_prob
                else
                    p->D.data[i][j]=0;
            }
        }
        p->A = p->A * p->D;
    }
    p->Z = p->W * p->A;
    for(i=0; i<p->Z.row; i++) //��������������ӣ�class��δд
    {
        for(j=0; j<p->Z.col; j++)
        {
            p->Z.data[i][j]+=p->b.data[i][0];//������԰�bҲ����Ϊ�ȴ�С�ľ���ÿ��һ��
        }
    }
}

void sigmoid_forward(parameters *p)
{
    int i,j;
    for(i=0; i<p->Z.row; i++)
    {
        for(j=0; j<p->Z.col; j++)
        {
            p->next->A.data[i][j]=1.0/(1.0+exp(-p->Z.data[i][j]));//sigmoid(p->Z.data[i][j]);
        }
    }
}

void relu_forward(parameters *p)
{
    int i,j;
    for(i=0; i<p->Z.row; i++)
    {
        for(j=0; j<p->Z.col; j++)
        {
            if(p->Z.data[i][j]>0)
            {
                p->next->A.data[i][j] = p->Z.data[i][j];
            }
            else
            {
                p->next->A.data[i][j]=0;
            }
        }
    }
}

void line_active_forward(parameters *p,string active, double keep_prob)
{
    line_forward(p,keep_prob);

    if(active=="relu")
    {
        relu_forward(p);
    }
    if(active=="sigmoid")
    {
        sigmoid_forward(p);
    }
}

Matrix model_forward(Matrix X,double *keep_probs)
{
    int i=0;
    int L=sup_par.layer_dims;

    parameters *p=&par;
    p->A = X;
    for(i=0; i<L-1&&p->next!=NULL; i++)
    {

        line_active_forward(p,sup_par.layer_active[i+1],keep_probs[i]);
        p=p->next;
    }
    return p->A;
}
void sigmoid_backword(parameters *p,grad *g)
{
    int i=0,j=0;
    for(i=0; i<g->grad_A.row; i++)
    {
        for(j=0; j<g->grad_A.col; j++)
        {
            g->grad_Z.data[i][j]=g->grad_A.data[i][j]*p->A.data[i][j]*(1-p->A.data[i][j]);
        }
    }
}

void relu_backword(parameters *p,grad *g)
{
    int i=0,j=0;
    for(i=0; i<g->grad_Z.row; i++)
    {
        for(j=0; j<g->grad_Z.col; j++)
        {
            if(p->pre->Z.data[i][j]>0)
            {
                g->grad_Z.data[i][j]=g->grad_A.data[i][j];
            }
            else
            {
                g->grad_Z.data[i][j]=0;
            }
        }
    }
}

void line_backword(parameters *p,grad *g, double lambd, double keep_prob)
{
    int i,j;
    //p->AT.transposematrix(p->A,&p->AT);
    Matrix AT = p->A.transposeMatrix();
    g->grad_W = g->grad_W.multsMatrix(g->grad_Z,AT);
    if(lambd!=0)
    {
        for(i=0; i<p->W.row; i++)
        {
            for(j=0; j<p->W.col; j++)
            {
                 g->grad_W.data[i][j]+=(lambd * p->W.data[i][j]);
            }
        }
    }
    for(i=0; i<g->grad_W.row; i++)
    {
        for(j=0; j<g->grad_W.col; j++)
        {
            g->grad_W.data[i][j]/=g->grad_Z.col;
        }
    }
    for(i=0; i<g->grad_Z.row; i++)
    {
        g->grad_b.data[i][0]=0;
        for(j=0; j<g->grad_Z.col; j++)
        {
            g->grad_b.data[i][0]+=g->grad_Z.data[i][j];
        }
        g->grad_b.data[i][0]/=g->grad_Z.col;
    }
    Matrix WT = p->W.transposeMatrix();
    g->pre->grad_A = g->pre->grad_A.multsMatrix(WT,g->grad_Z);

    if(keep_prob!=1)
    {
        //����pָ���D���ӦA��dropout�㣬������1������£�D��ֻ�г�ʼ�����޹ظ�ֵ�����Զ�Ӧdropout��ϵ����ȷ��
        //cout<<p->D.col<<"&"<<p->D.row<<endl;
        //cout<<g->pre->grad_A.col<<"&"<<g->pre->grad_A.row<<endl;

        g->pre->grad_A = g->pre->grad_A.multsMatrix(g->pre->grad_A,p->D);//����keep_prob�����Ѿ��ŵ�D����

        //p->D.print(p->next->D);
        //cin>>i;
    }
    //AT.clear();
    //WT.clear();
}

void line_active_backword(parameters *p,grad *g,string active, double lambd, double keep_prob)
{
    if(active=="sigmoid")
    {
        sigmoid_backword(p,g);
    }
    if(active=="relu")
    {
        relu_backword(p,g);
    }
    line_backword(p->pre,g,lambd,keep_prob);
}
void model_backword(Matrix AL,Matrix Y,double lambd,double *keep_probs)
{
    int i=0;
    int L=sup_par.layer_dims;

    parameters *p=&par;
    while(p->next!=NULL)
    {
        p=p->next;
    }
    grad *g=&gra;

    for(i=0; i<Y.col; i++)
    {
        gra.grad_A.data[0][i]=-(Y.data[0][i]/AL.data[0][i]-(1-Y.data[0][i])/(1-AL.data[0][i]));
    }
    for(i=L-1; i>0; i--)
    {
        line_active_backword(p,g,sup_par.layer_active[i],lambd,keep_probs[i]);
        g=g->pre;
        p=p->pre;
    }
}

double cost_cumpter(Matrix AL,Matrix Y,double lambd)
{
    int i=0,j=0;
    int m=Y.col;//������
    double loss=0;
    double loss_L2_regularization=0;
    if(lambd!=0)
    {
        parameters *p=&par;
        while(p!=NULL)
        {
            for(i=0;i<p->W.row;i++)
            {
                for(j=0;j<p->W.col;j++)
                {
                    loss_L2_regularization+=(lambd*p->W.data[i][j]*p->W.data[i][j]);
                }
            }
           p=p->next;
        }
        loss_L2_regularization/=(2*m);
    }
    for(i=0; i<m; i++)
    {
        loss+=-(Y.data[0][i]*log(AL.data[0][i])+(1-Y.data[0][i])*log(1-AL.data[0][i]));
    }
    loss/=m;
    //loss+=loss_L2_regularization;
    return loss;
}
int updata_parameters_with_gd(double learn_rateing, int t)
{
    int k=0,i=0,j=0;
    int L=sup_par.layer_dims;
    parameters *p=&par;
    grad *g=&gra;
    while(g->pre->pre!=NULL)//�����ǰ����ϲ����ķ�ʽ��ͬ��ǰ��A=sgn(WX+b),���� AL=sgn(WA+b)
    {
        g=g->pre;
    }

    //learn_rateing *= pow(0.999, t/1000);//ָ��˥��

    for(k=0; k<L-1&&p->next!=NULL&&g!=NULL; k++)
    {
        for(i=0; i<g->grad_W.row; i++)
        {
            g->grad_b.data[i][0]*=-learn_rateing;
            for(j=0; j<g->grad_W.col; j++)
            {
                g->grad_W.data[i][j]*=-learn_rateing;
            }
        }
        p->W = p->W + g->grad_W;
        p->b = p->b + g->grad_b;
        p=p->next;
        g=g->next;
    }
    return 0;
}

int updata_parameters_with_momentum(double learn_rateing, int t,double beta)
{
    int k=0,i=0,j=0;
    int L=sup_par.layer_dims;
    parameters *p=&par;
    grad *g=&gra;
    while(g->pre->pre!=NULL)//�����ǰ����ϲ����ķ�ʽ��ͬ��ǰ��A=sgn(WX+b),���� AL=sgn(WA+b)
    {
        g=g->pre;
    }

    //learn_rateing *= pow(0.999, t/1000);//ָ��˥��

    for(k=0; k<L-1&&p->next!=NULL&&g!=NULL; k++)
    {
        for(i=0; i<g->grad_W.row; i++)
        {
            g->V_db.data[i][0]=(beta * g->V_db.data[i][0] + (1-beta) * g->grad_b.data[i][0]);
            g->V_db_corrected.data[i][0] = g->V_db.data[i][0] / (1-pow(beta,t));//����
            g->grad_b.data[i][0]=(-learn_rateing) * g->V_db_corrected.data[i][0];

            for(j=0; j<g->grad_W.col; j++)
            {
                g->V_dw.data[i][j]=(beta * g->V_dw.data[i][j] + (1-beta) * g->grad_W.data[i][j]);
                g->V_dw_corrected.data[i][j]=g->V_dw.data[i][j] / (1-pow(beta,t));//����
                g->grad_W.data[i][j]=(-learn_rateing) * g->V_dw_corrected.data[i][j];
            }
        }
        p->W = p->W + g->grad_W;
        p->b = p->b + g->grad_b;
        p=p->next;
        g=g->next;
    }
    return 0;
}

int updata_parameters_with_adam(double learn_rateing, int t, double beta1, double beta2, double epsilon)
{
    int k=0,i=0,j=0;
    int L=sup_par.layer_dims;
    parameters *p=&par;
    grad *g=&gra;
    while(g->pre->pre!=NULL)//�����ǰ����ϲ����ķ�ʽ��ͬ��ǰ��A=sgn(WX+b),���� AL=sgn(WA+b)
    {
        g=g->pre;
    }

    //learn_rateing *= pow(0.999, t/1000);//ָ��˥��

    for(k=0; k<L-1&&p->next!=NULL&&g!=NULL; k++)
    {
        for(i=0; i<g->grad_W.row; i++)
        {
            g->V_db.data[i][0]=(beta1 * g->V_db.data[i][0] + (1-beta1) * g->grad_b.data[i][0]);
            g->V_db_corrected.data[i][0] = g->V_db.data[i][0] / (1-pow(beta1,t));//����

            g->S_db.data[i][0]=(beta2 * g->S_db.data[i][0] + (1-beta2) * (g->grad_b.data[i][0] * g->grad_b.data[i][0]));
            g->S_db_corrected.data[i][0] = g->S_db.data[i][0] / (1-pow(beta2,t));//����

            g->grad_b.data[i][0]= (-learn_rateing) * g->V_db_corrected.data[i][0] / sqrt(g->S_db_corrected.data[i][0]);

            for(j=0; j<g->grad_W.col; j++)
            {
                g->V_dw.data[i][j]=(beta1 * g->V_dw.data[i][j] + (1-beta1) * g->grad_W.data[i][j]);
                g->V_dw_corrected.data[i][j]=g->V_dw.data[i][j] / (1-pow(beta1,t));//����

                g->S_dw.data[i][j]=(beta2 * g->S_dw.data[i][j] + (1-beta2) * (g->grad_W.data[i][j] * g->grad_W.data[i][j]));
                g->S_dw_corrected.data[i][j]=g->S_dw.data[i][j] / (1-pow(beta2,t));//����

                g->grad_W.data[i][j]= (-learn_rateing) * g->V_dw_corrected.data[i][j] / sqrt(g->S_dw_corrected.data[i][j]+epsilon) ;
            }
        }
        p->W = p->W + g->grad_W;
        p->b = p->b + g->grad_b;
        p=p->next;
        g=g->next;
    }
    return 0;
}

int updata_parameters(double learn_rateing, int t, const char *optimizer, double beta1, double beta2, double epsilon)
{

    if(optimizer=="gd")
       updata_parameters_with_gd(learn_rateing, t);
    else if(optimizer="momentum")
        updata_parameters_with_momentum(learn_rateing, t, beta1);
    else if(optimizer="adam")
        updata_parameters_with_adam(learn_rateing, t, beta1, beta2, epsilon);
    return 0;
}
int predict(Matrix X,Matrix Y)
{
    int i,k;
    parameters *p;
    p=&par;
    p->A = X.copyMatrix();
    Matrix AL;
    double *keep_probs=new double [sup_par.layer_dims];
    for(k=0;k<sup_par.layer_dims;k++)
    {
        keep_probs[k]=1;
    }
    AL=model_forward(X,keep_probs);
    for(i=0;i<Y.col;i++)
    {
        if(AL.data[0][i]>0.5)
            AL.data[0][i]=1;
        else
            AL.data[0][i]=0;
    }
    double pre=0;
    for(i=0;i<Y.col;i++)
    {
        if((AL.data[0][i]==1 && Y.data[0][i]==1)||(AL.data[0][i]==0 && Y.data[0][i]==0))
            pre+=1;
    }
    pre/=Y.col;
    cout<<"pre="<<pre<<endl;
    return 0;
}

int DNN(Matrix X,Matrix Y,const char *optimizer,double learn_rateing,const char *initialization, double lambd, double keep_prob, \
        int mini_batch_size,double beta1, double beta2, double epsilon, int iter, bool print_cost)
{
    /**
    ��ʼ������

    **/
    int i=0,k=0;
    int lay_dim=3;
    int lay_n[3]= {0,3,1};
    lay_n[0]=X.row;
    string lay_active[3]= {"relu","relu","sigmoid"};
    sup_par.layer_dims=lay_dim;
    for(i=0; i<lay_dim; i++)
    {
        sup_par.layer_n[i]=lay_n[i];
        sup_par.layer_active[i]=lay_active[i];
    }
    init_parameters(X,initialization);
    double loss;

    Matrix AL(Y.row,Y.col,0,"ss");
    double *keep_probs;
    if(keep_prob==1)
    {
        keep_probs=new double (sup_par.layer_dims);
        for(k=0;k<sup_par.layer_dims;k++)
        {
            keep_probs[k]=1;
        }
    }
    else if (keep_prob<1)
    {
        keep_probs=new double (sup_par.layer_dims);
        for(k=0;k<sup_par.layer_dims;k++)
        {
            if(k==0 || k==sup_par.layer_dims-1)
            {
                keep_probs[k]=1;
            }
            else
            {
                keep_probs[k]=1;
            }
        }
    }
    for(i=0; i<iter; i++)
    {
        //cout<<"-----------forward------------"<<"i="<<i<<endl;
        AL=model_forward(X,keep_probs);
        //cout<<"-----------loss--------------"<<endl;
        loss=cost_cumpter(AL,Y,lambd);
        if(i%100==0)
            cout<<"loss="<<loss<<endl;
        //cout<<"-----------backword-----------"<<endl;
        model_backword(AL,Y,lambd,keep_probs);

        //cout<<"-----------update--------------"<<endl;
        updata_parameters(learn_rateing,i+1,optimizer,beta1,beta2,epsilon);
    }
    predict(X,Y);
    return 0;
}
int trainDNN()
{
    Matrix x;
    char file[20]="data\\train.txt";
    x.LoadData(file);
    x = x.transposeMatrix();
    //x.print();

    cout<<"x,y"<<endl;
    cout<<"----------------------"<<endl;
    Matrix y;
    cout<<x.row<<endl;
    y=x.getOneRow(x.row-1);
    x.deleteOneRow(x.row-1);
    y=one_hot(y,2);
    cout<<x.row<<"*"<<x.col<<endl;
    cout<<y.row<<"*"<<y.col<<endl;
    /**
        ��һ������Ҫ
    **/
    /*int i=0,j=0;
    for(i=0;i<X.col;i++)
    {
        for(j=0;j<X.row;j++)
        {
            X.mat[i][j]/=255;
        }
    }*/
    const char *initialization="he";
    double learn_rateing=0.1;
    int iter=1000;
    double lambd=0.1;
    double keep_prob=0.5;
    bool print_cost=true;
    const char *optimizer="gd";
    int mini_batch_size=64;
    double beta1=0.9;
    double beta2=0.999;
    double epsilon=0.00000001;

    DNN(x,y,optimizer="gd",learn_rateing=0.001,initialization="he",lambd=0.001,keep_prob = 1,mini_batch_size=64, \
        beta1=0.9, beta2=0.999, epsilon=0.00000001, iter=5000, print_cost=true);
    predict(x,y);
    return 0;
}


