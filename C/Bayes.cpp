#include <io.h>
#include <string.h>
#include <stdio.h>
#include "Bayes.h"

#define CLASS_SUM 2
#define X_RANGE 2  //X_RANGE ��ʾX��ȡֵ��Χ�����ڲ���one-hot���룬����X��ÿһ��������ȡֵ��ΧΪ0��1

struct Par
{
    Matrix pX_1Y;
    Matrix pY;
    Matrix pX;
};

Par bayes;//����bayes���������еĲ���

DIC createVocabList(vector<DataStr> dataClass)//���ɵ����ֵ�
{
    int i,j,k,vl;
    vector<string> dic;
    for(k=0; k<dataClass.size(); k++)
    {
        for(i=0; i<dataClass[k].size(); i++)
        {
            for(j=0; j<dataClass[k][i].size(); j++)
            {
                for(vl=0; vl<dic.size(); vl++)
                {
                    //cout<<dataClass[k][i][j]<<endl;
                    if(!dataClass[k][i][j].compare(dic[vl]))
                        break;
                }
                if(vl==dic.size())
                {
                    dic.push_back(dataClass[k][i][j]);
                }
            }
        }
    }
    return dic;
}

Matrix createFectVec(vector<DataStr> dataClass,DIC dic)//����ѵ����������
{
    int i,j,k=0,vl;
    int sampleNum=0;
    for(k=0; k<dataClass.size(); k++)
    {
        sampleNum+=dataClass[k].size();
    }
    Matrix vecX;
    vecX.initMatrix(sampleNum,dic.size()+1,0,"ss");
    int iSample=0;
    for(k=0; k<dataClass.size(); k++)
    {
        for(i=0; i<dataClass[k].size(); i++)//ֻ��һ��
        {
            vecX.data[iSample][dic.size()]=k;//��ǩ
            for(j=0; j<dataClass[k][i].size(); j++)
            {
                for(vl=0; vl<dic.size(); vl++)
                {
                    if(!dataClass[k][i][j].compare(dic[vl]))
                    {
                        vecX.data[iSample][vl]=1;//one-hot��������
                        break;
                    }
                }
            }
            iSample++;
        }
    }
    return vecX;
}

int trainNB(Matrix X,Matrix Y)//ѵ��������ʵ�֣�ע��Բ�������ƽ������
{
    //�࣬����������ȡֵ
    bayes.pY.initMatrix(CLASS_SUM,1,0,"ss");//�����ʼ��Ϊ2�е�������
    bayes.pX_1Y.initMatrix(CLASS_SUM,X.col,0,"ss");//X_1Y��ʾ��Y��X=1�ĸ��ʣ���֮X=0�ĸ���Ϊ1-
    bayes.pX.initMatrix(X.col,1,0,"ss");//

    int i,j,k;
    for(k=0; k<bayes.pX_1Y.row; k++)
    {
        for(i=0; i<bayes.pX_1Y.col; i++)
        {
            bayes.pX_1Y.data[k][i]=1;//ƽ������,Ĭ�ϳ���һ�Σ����ڹ�һ��ʱ�����������ĳ���Ҳ���ǽ�ȥ,�����ƽ����ָÿһ���ֵ��������ֵ�ģ�δ�漰����
            //�����е�δ�����
        }
    }
    for(i=0; i<X.row; i++)
    {
        if(Y.data[i][0]==0)
        {
            bayes.pY.data[0][0]++;
            for(j=0; j<X.col; j++)
            {
                bayes.pX_1Y.data[0][j]+=X.data[i][j];
            }
        }
        else
        {
            bayes.pY.data[1][0]++;
            for(j=0; j<X.col; j++)
            {
                bayes.pX_1Y.data[1][j]+=X.data[i][j];
            }
        }
    }
    for(i=0; i<X.col; i++)
    {
        //��������x���������������ֵĸ���
        //bayes.pX.data[i][0]=(bayes.pX_1Y.data[0][i]-1) + (bayes.pX_1Y.data[1][i]-1) + 1;
        //bayes.pX.data[i][0]/=bayes.pY.data[0][0] + bayes.pY.data[1][0] + 2;

        //ĳһ����x���������������ֵĸ���
        bayes.pX_1Y.data[0][i]/=bayes.pY.data[0][0] + 2;
        bayes.pX_1Y.data[1][i]/=bayes.pY.data[1][0] + 2;

        bayes.pX.data[i][0] = (bayes.pX_1Y.data[0][i] + bayes.pX_1Y.data[1][i])/2;
    }
    //�����PY����ĸ���
    for(k=0; k<bayes.pY.row; k++)
    {
        bayes.pY.data[k][0]/=X.row;
    }
    cout<<"pY="<<bayes.pY.data[0][0]<<endl;

    for(k=0; k<bayes.pX_1Y.row; k++)
    {
        for(i=0; i<bayes.pX_1Y.col; i++)
        {
            cout<<bayes.pX_1Y.data[k][i]<<" & ";
        }
        cout<<"---";
    }
}

/**
������ߺ���
**/
int classifyNB(Matrix testVecX)
{
    double p0=1.0,p1=1.0;
    int i,j;
    for(i=0; i<testVecX.row; i++)
    {
        p0=1.0,p1=1.0;
        for(j=0; j<testVecX.col ; j++)
        {
            //�������������ֵĸ���Ϊ1-
            if(testVecX.data[i][j]==0)
            {
                p0*=(1-bayes.pX_1Y.data[0][j])/(1-bayes.pX.data[j][0]);
                p1*=(1-bayes.pX_1Y.data[1][j])/(1-bayes.pX.data[j][0]);
            }
            //�����������ֵĸ���
            else
            {
                p0*=(bayes.pX_1Y.data[0][j])/bayes.pX.data[j][0];
                p1*=(bayes.pX_1Y.data[1][j])/bayes.pX.data[j][0];
            }
        }
        p0*=bayes.pY.data[0][0];
        p1*=bayes.pY.data[1][0];
        cout<<"p0="<<p0<<"&"<<"p1="<<p1;
        if(p0<p1)
            cout<<"class="<<1;
        else
            cout<<"class="<<0;
        cout<<endl;
    }
    return 0;

}
void getAllFiles(string path, vector <string>&files)
{
    //�ļ����
    long   hFile = 0;
    //�ļ���Ϣ
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //�����Ŀ¼,����֮
            //�������,�����б�
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        }
        while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}
void Bayes()
{

    int k,i,j;
    string path[CLASS_SUM]= {"data\\email\\ham\\","data\\email\\spam\\"};
    vector<string> files;
    vector< vector<string> > filess;
    RowDataStr rowData;
    DataStr data;
    vector<DataStr> dataClass;
    for(k=0; k<CLASS_SUM; k++)
    {
        getAllFiles(path[k],files);
        filess.push_back(files);
        files.clear();
    }
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    for(k=0; k<filess.size(); k++)
    {
        for(i=0; i<filess[k].size(); i++)
        {
            LoadDataStr(rowData,filess[k][i].c_str());
            data.push_back(rowData);
            rowData.clear();
        }
        dataClass.push_back(data);
        data.clear();
    }
    DIC dic=createVocabList(dataClass);
    cout<<dic.size();
    /**
    ������������
    **/
    Matrix X=createFectVec(dataClass,dic);

    /**
    ����������ǩ
    **/
    Matrix Y;
    Y = X.getOneCol(X.col-1);
    X.deleteOneCol(X.col-1);
    cout<<X.row<<"*"<<X.col<<endl;
    cout<<Y.row<<"*"<<Y.col<<endl;
    Y.print();
    trainNB(X,Y);///ͨ����������ͱ�ǩ������ò�����⺯�����bayes���ߵĲ���

    classifyNB(X);///�������

    //bayes.pX_1Y.print();
    cout<<"----------"<<endl;
    Matrix pxt=bayes.pX.transposeMatrix();
    pxt.print();
}

