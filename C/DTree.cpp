/***
��������ID3,C4.5,CART

����

1��Ϣ����,��Ϣ������,����ָ��
2�������ͣ�string��
3Ԥ��֦
**/

#include "DTree.h"
#define MAX_SIZE_OF_TRAINING_SET 1000
using namespace std;

struct Tree
{
    int id;
    int split_feat;//��������
    string feat_value;//������ֵ
    string label;//�������
    int size;
    int kind;
    int parent;
    Tree *next;
};

Tree tree[MAX_SIZE_OF_TRAINING_SET];//�����洢�������������������
stack<Tree> s;
int node=0;//�����洢�������������������������±�
vector<int> feat_flag;//��ʼ��Ϊ0,���һ������ͳ�ƻ�ʣ��������δʹ��

vector<string> getkindattr(const DataStr &data,int axis)//���axis���Ե����в�ͬȡֵ
{
    unsigned int i,j;
    vector<string> attr_value;
    for(i=0; i<data.size(); i++)
    {
        for(j=0; j<attr_value.size(); j++)
        {
            if(data[i][axis]==attr_value[j])
                break;
        }
        if(j==attr_value.size())
        {
            attr_value.push_back(data[i][axis]);
        }
    }
    return attr_value;
}

double calcShannonEntOrGini(const DataStr &data, const string &type)
{
    unsigned int i;
    map<string,double> label_value;//ÿһ�������ĸ���ͳ��
    double prob=0;
    double shannoEnt=0;
    double Gini=1;
    string label;
    for(i=0; i<data.size(); i++)
    {
        label=data[i][data[0].size()-1];
        if(label_value.count(label))
            label_value[label]+=1;//�������ѳ�ʼ������֮ǰ�Ļ����ϼ�1
        else
            label_value[label]=1;//������δ��ʼ������ѵ�ǰ�������������ʼ��Ϊ1
    }
    //ͳ�Ƹ������ݼ��ϵ����������ľ��������Լ���ͬ����������������
    for(map<string,double>::iterator it=label_value.begin();it!=label_value.end();it++)
    {
        prob=it->second/data.size();
        shannoEnt-=prob*log(prob);
        Gini-=prob*prob;
    }
    //cout<<"shannoEnt="<<shannoEnt<<endl; //�����Ϣ�ؼ����Ƿ���ȷ
    if(type=="Gini")
        return Gini;
    if(type=="Ent")
        return shannoEnt;
    return shannoEnt;
}

DataStr splitDataSet(const DataStr &data,int axis,string value)
{
    DataStr subData;
    int i;
    for(i=0; i<data.size(); i++)
    {
        if(data[i][axis]==value)
        {
            subData.push_back(data[i]);
        }
    }
    return subData;
}

int chooseBestFectureTosplit(const DataStr &data, const string &type, double epsilon, int minLen)
{
    int i,j,k;
    double baseEnt;
    double baseGini;
    baseEnt=calcShannonEntOrGini(data,"Ent");
    baseGini=calcShannonEntOrGini(data,"Gini");
    int bestFeature=-1;
    double newEnt=0;
    double newGini=0;
    double bestInfoGain=0;
    double bestgainRatio=0;
    double bestGini=0;
    double infoGain=0;
    double splitInfo=0;
    double gainRatio=0;
    double giniGain=0;
    double prob=0;
    double len=0;
    DataStr subData;
    vector<vector<string> > feat_value(data[0].size()-1);
    for(i=0; i<data.size(); i++)//��ʼ��ÿ�����ԵĲ�ͬȡֵ�Ķ�ά����
    {
        for(j=0; j<data[0].size()-1; j++)
        {
            if(feat_flag[j]!=1)
                feat_value[j]=getkindattr(data,j);
            else
                feat_value[j].push_back("");
        }
    }
    for(i=0; i<data[0].size()-1; i++)
    {
        if(feat_flag[i]!=1)
        {
            cout<<"---------"<<"feat="<<i<<"--------"<<endl;
            newEnt=0;
            newGini=0;
            splitInfo=0;
            for(j=0; j<feat_value[i].size(); j++)
            {
                subData=splitDataSet(data,i,feat_value[i][j]);
                if(subData.size()<minLen)
                {
                    newGini=100;
                    newEnt=100;//��������ѭ���󣬰Ѵ˴ε���Ϣ����ΪMAX������ѡ�������
                    break;//Ԥ��֦����
                }
                prob=double(subData.size())/double(data.size());
                splitInfo+=(-log(prob)*prob);//������������Ϣ��
                //cout<<"splitInfo="<<splitInfo<<endl;
                newEnt+=prob*calcShannonEntOrGini(subData,"Ent");
                newGini+=prob*calcShannonEntOrGini(subData,"Gini");
            }
            infoGain=baseEnt-newEnt;//������Ϣ����
            gainRatio=infoGain/splitInfo;//������Ϣ������
            giniGain=baseGini-newGini;//����Gini����
            if(type=="ID3")
            {
                cout<<"fet="<<i<<"infogain="<<infoGain<<endl;
                if(infoGain>bestInfoGain&&infoGain>epsilon)//��Ϣ������������ֵ���ſ����������Է���
                {
                    bestInfoGain=infoGain;
                    bestFeature=i;
                }
            }
            if(type=="C4.5")
            {
                cout<<"fet="<<i<<"gainRatio="<<gainRatio<<endl;
                if(gainRatio>bestgainRatio&&gainRatio>epsilon)//��Ϣ������������ֵ���ſ����������Է���
                {
                    bestgainRatio=gainRatio;
                    bestFeature=i;
                }
            }
            if(type=="CART")
            {
                cout<<"fet="<<i<<"giniGain="<<giniGain<<endl;
                if(giniGain>bestGini&&giniGain>epsilon)//��Ϣ������������ֵ���ſ����������Է���
                {
                    bestGini=giniGain;
                    bestFeature=i;
                }
            }
        }
    }
    cout<<"bestFet="<<bestFeature<<endl;
    return bestFeature;
}

Tree dataToTree(const DataStr &data,const string &type, int bbestFet)
{
    int i,j;
    int signalflag=1;//���������Ƿ�ͬ��������Ƿ�ȫ�����־
    int bestFet=0;
    for(i=0; i<data.size(); i++)
    {
        if(data[i][data[0].size()-1]!=data[0][data[0].size()-1])
            signalflag=0;
    }
    if(signalflag)//ֻ��һ������
    {
        tree[node].label=data[0][data[0].size()-1];//�����Ϊ��һ��������label
        tree[node].split_feat=-1;
        tree[node].feat_value="";
        tree[node].parent=bbestFet;
        tree[node].size=data.size();
        tree[node].id=node;
        cout<<"fetflag=1----"<<tree[node].label<<endl;
        node++;
        return tree[node];
    }
    for(i=0; i<data[0].size()-1; i++)
        feat_flag[data[0].size()-1]+=feat_flag[i];
    if(feat_flag[data[0].size()-1]==feat_flag.size()-1)//�������Զ�ʹ������
    {
        tree[node].label=data[0][data[0].size()-1];//�����Ϊ��һ��������label
        tree[node].split_feat=-1;
        tree[node].feat_value="";
        tree[node].parent=bbestFet;
        tree[node].size=data.size();
        tree[node].id=node;
        cout<<"fetflag=1----"<<tree[node].label<<endl;
        node++;
        return tree[node];
    }
    bestFet=chooseBestFectureTosplit(data,type,0.01,1);
    if(bestFet==-1)//���������Ϣ����С��Ԥ�����С��Ϣ���������������������С��Ԥ�����С��Ҳ����
    {
        tree[node].label=data[0][data[0].size()-1]; //������Ϊ��һ��������label
        tree[node].split_feat=-1;
        tree[node].feat_value="";
        tree[node].parent=bbestFet;
        tree[node].size=data.size();
        tree[node].id=node;
        cout<<"fetflag=1----"<<tree[node].label<<endl;
        node++;
        return tree[node];
    }
    feat_flag[bestFet]=1;//ѡ����õķ�������֮���ڷ��ط���֮ǰ�������ٿ����ø����Խ��з���
    vector<string> feat_value=getkindattr(data,bestFet);//��õ�ǰ���������ж�����ȡֵ
    for(j=0; j<feat_value.size(); j++)//���Ѻ���Ӽ���������ͬ��Ҳ�����ȶ���������Ӽ��Ƚ��н�����������ߵ�ʱ���ܸ�����ʵ����ҵ�ƥ���
    {
        tree[node]=dataToTree(splitDataSet(data,bestFet,feat_value[j]), type, bestFet);
    }
    feat_flag[bestFet]=0;//ÿ�η�����һ�ڵ�ʱ����֮ǰ���ѵ����Դ��»��գ����������´η���
    tree[node].feat_value=data[0][data[0].size()-1]; //��������Ϊ��һ��������label
    tree[node].split_feat=bestFet;
    tree[node].parent=bbestFet;
    tree[node].label="-1";
    tree[node].size=data.size();
    tree[node].id=node;
    node++;
    return tree[node];
}
int createTree()
{
    int i;
    int len;
    Tree *child;
    Tree *p;
    for(i=0;i<node;i++)
    {
        if(tree[i].split_feat==-1)
        {
            s.push(tree[i]);
        }
        else
        {
            len=tree[i].size;
            p=&tree[i];
            while((len))
            {
                len-=s.top().size;
                child=new Tree;
                *child=s.top();
                s.pop();
                p->next=child;
                p=p->next;
            }
            s.push(tree[i]);
        }
    }
    s.pop();
    return 0;
}
int DTree(const string &type)
{
    DataStr data;
    LoadDataStr(data,"data\\id3.txt");
    for(unsigned int i=0;i<data[0].size();i++)
        feat_flag.push_back(0);
    dataToTree(data,type,0);//��ֵ����Ҫ�������ص����ڵ�ʱ����ȡ-1������޷����ʸ��ڵ��ϵ���������ֵ
    createTree();
    Tree *p;
    for(int i=0;i<node;i++)
    {
        p=&tree[i];
        while(p!=NULL)
        {
            cout<<p->id<<"  ";
            p=p->next;
        }
        cout<<endl;
    }
    return 0;
}



