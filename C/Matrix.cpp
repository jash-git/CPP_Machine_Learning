#include "Matrix.h"
using namespace std;
Matrix::Matrix()
{
    //Matrix(0);
}
Matrix::Matrix(unsigned int row, unsigned int col,float lamd, const char* type)
{
    RowData cda(col);
    Data da(row,cda);
    this->data=da;
    this->row=row;
    this->col=col;
    unsigned int i=0,j=0;
    if(type!="diag")
    {
        for(i=0; i<row; i++)
        {
            for(j=0; j<col; j++)
                data[i][j]=lamd;
        }
    }
    else
    {
        if(row==col)
        {
            for(i=0; i<col; i++)
                data[i][i]=lamd;
        }
        else
        {
            cout<<"diag must row=col"<<endl;
        }
    }
}
void Matrix::initMatrix(unsigned int row, unsigned int col,float lamd, const char* type)
{
    if(row==0 || col==0)
    {
        cout<<"no zero"<<endl;
        return ;
    }
    RowData cda(col);
    Data da(row,cda);
    this->data=da;
    this->row=row;
    this->col=col;
    unsigned int i=0,j=0;
    if(type!="diag")
    {
        for(i=0; i<row; i++)
        {
            for(j=0; j<col; j++)
                data[i][j]=lamd;
        }
    }
    else
    {
        if(row==col)
        {
            for(i=0; i<row; i++)
                data[i][i]=lamd;
        }
        else
        {
            cout<<"diag must row=col"<<endl;
        }
    }
}
void Matrix::LoadData(const char *filename)
{
    LoadDataNum(this->data,filename);
    row=data.size();
    col=data[0].size();
}
void Matrix::print()
{
    unsigned int i,j;
    for(i=0; i<row; i++)
    {
        for(j=0; j<col; j++)
        {
            cout<<data[i][j]<<"  ";;
        }
        cout<<endl;
    }
}
Matrix Matrix::copyMatrix()
{
    unsigned int i=0,j=0;
    Matrix cp;
    RowData cda(this->col);
    Data da(this->row,cda);
    cp.data=da;
    cp.col=this->col;
    cp.row=this->row;
    for(i=0; i<this->row; i++)
    {
        for(j=0; j<this->col; j++)
        {
            cp.data[i][j]=this->data[i][j];
        }
    }
    return cp;
}
Matrix Matrix::getOneRow(unsigned int iRow)
{
    //this->data(matrix.data.size());
    unsigned int j=0;
    RowData cda(col);
    Data da(1,cda);
    Matrix matrix;
    matrix.data=da;
    matrix.col=col;
    matrix.row=1;
    for(j=0; j<this->data[iRow].size(); j++)
    {
        matrix.data[0][j]=this->data[iRow][j];
    }

    return matrix;
}
Matrix Matrix::getOneCol(unsigned int jCol)
{
    unsigned int i=0;
    ColData cda(1);
    Data da(row,cda);
    Matrix matrix;
    matrix.data=da;
    matrix.col=1;
    matrix.row=row;
    for(i=0; i<this->data.size(); i++)
    {
        //cout<<i<<"="<<this->data[i][jCol]<<endl;
        matrix.data[i][0]=this->data[i][jCol];

    }
    return matrix;
}
void Matrix::deleteOneRow(unsigned int iRow)
{
    unsigned int i=0;
    for(Data::iterator it=data.begin(); it!=data.end(); it++,i++)
    {
        if(i==iRow)
        {
            data.erase(it);
        }
    }
    this->row--;
}
void Matrix::deleteOneCol(unsigned int iCol)
{

    unsigned int i=0,j=0;
    Matrix cp=this->copyMatrix();

    this->col--;
    this->data.clear();
    RowData cda(this->col);
    Data da(this->row,cda);
    this->data=da;

    for(Data::iterator it=cp.data.begin(); it!=cp.data.end(); it++,i++)
    {
        j=0;
        for(vector<float>::iterator itRow=cp.data[i].begin(); itRow!=cp.data[i].end(); itRow++,j++)
        {
            if(j<iCol)
            {
                this->data[i][j]=*itRow;
            }
            if(j>iCol)
            {
                this->data[i][j-1]=*itRow;
            }
        }

    }
}
Matrix Matrix::transposeMatrix()//������ʽ��ת��
{
    unsigned int i=0,j=0;
    Matrix matrixT;
    ColData cda(row);
    Data da(col,cda);
    matrixT.data=da;
    matrixT.row=col;
    matrixT.col=row;
    for(i=0; i<col; i++)
    {
        for(j=0; j<row; j++)
        {
            matrixT.data[i][j]=data[j][i];
        }
    }
    return matrixT;
}
Matrix Matrix::addMatrix(const Matrix &matrix1,const Matrix &matrix2)
{
    if(matrix1.col!=matrix2.col||matrix1.row!=matrix2.row)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"addData data1 data2 is no"<<endl;
        exit(-1);
    }
    RowData cda(matrix1.col);
    Data da(matrix1.row,cda);
    Matrix add;
    add.data=da;
    add.row=matrix1.row;
    add.col=matrix1.col;
    unsigned int i,j;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix1.col; j++)
        {
            add.data[i][j]=matrix1.data[i][j]+matrix2.data[i][j];
        }
    }

    return add;
}

Matrix Matrix::subMatrix(const Matrix &matrix1,const Matrix &matrix2)
{
    if(matrix1.col!=matrix2.col||matrix1.row!=matrix2.row)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"subData data1 data2 is no"<<endl;
        exit(-1);
    }
    RowData cda(matrix1.col);
    Data da(matrix1.row,cda);
    Matrix sub;
    sub.data=da;
    sub.row=matrix1.row;
    sub.col=matrix1.col;
    unsigned int i,j;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix1.col; j++)
        {
            sub.data[i][j]=matrix1.data[i][j]-matrix2.data[i][j];
        }
    }
    cout<<"oooooook"<<endl;
    //delete this;
    return sub;
}

Matrix Matrix::multsMatrix(Matrix matrix1,Matrix matrix2)//������ʽ�����
{
    if(matrix1.col!=matrix2.row)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"multsData error"<<endl;
        exit(-1);
    }
    unsigned int i,j,k;
    Matrix mults;
    ColData cda(matrix2.col);
    Data da(matrix1.row,cda);
    mults.data=da;
    mults.row=matrix1.row;
    mults.col=matrix2.col;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix2.col; j++)
        {
            mults.data[i][j]=0;
        }
    }
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix2.col; j++)
        {
            for(k=0; k<matrix1.col; k++)
            {
                mults.data[i][j]+=matrix1.data[i][k]*matrix2.data[k][j];
            }
        }
    }
    return mults;
}

Matrix Matrix::dotmultsMatrix(Matrix matrix1,Matrix matrix2)//������ʽ�����
{
    if(matrix1.row!=matrix2.row || matrix1.col!=matrix2.col)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"multsData error"<<endl;
        exit(-1);
    }
    unsigned int i,j;
    Matrix dotmults;
    ColData cda(matrix1.col);
    Data da(matrix1.row,cda);
    dotmults.data=da;
    dotmults.row=matrix1.row;
    dotmults.col=matrix1.col;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix2.col; j++)
        {
            dotmults.data[i][j]=matrix1.data[i][j] * matrix2.data[i][j];
        }
    }
    return dotmults;
}

//����ʽ
double Matrix::detMatrix()
{
    if(row!=col)
    {
        cout<<"Data det is no"<<endl;
        exit(-1);
    }
    Matrix mCopy=*this;
    double det=1;
    unsigned int i=0,j=0,k=0;
    double max=-9999999;
    int swap=-1;
    double temp;
    ColData cda(col);
    Data aij(row,cda);
    for(k=0; k<mCopy.col-1; k++)//k��ʾ��k����Ԫ��һ����Ҫn-1��
    {
        for(i=0; i<mCopy.row; i++)
        {
            if(mCopy.data[i][k]>max)//ÿһ����Ԫ���ǱȽϵ�k�е�Ԫ�أ�ѡ����k��������һ��
            {
                swap=i;
            }
        }//�ҵ���k������Ԫ��ȥ������е��±�
        if(swap==-1||mCopy.data[swap][k]==0)
            return -1;//�����ԪΪ0
        for(j=0; j<mCopy.col; j++)
        {
            temp=mCopy.data[k][j];
            mCopy.data[k][j]=mCopy.data[swap][j];
            mCopy.data[swap][j]=temp;
        }//��k����Ԫ��ѡ������һ����swap�У����k�н���
        for(i=k+1; i<mCopy.row; i++)
        {
            aij[i][k]=mCopy.data[i][k]/mCopy.data[k][k];// ��k����Ԫ����Ԫ��Ϊ��k�е�k�У��ѵ�k�����µ��ж�������Ԫ
            for(j=k; j<mCopy.col; j++)//����k�����µ�ÿһ�е�ÿһ��Ԫ�ض���ȥ��������Ԫ���ӵĳ˻�
            {
                mCopy.data[i][j]-=aij[i][k]*mCopy.data[k][j];
            }
        }
    }
    for(i=0; i<mCopy.row; i++)
    {
        det*=mCopy.data[i][i];
    }
    //cout<<"det="<<det<<endl;
    return det;
}
//��˹��Ԫ��������,�ر�ע�⣬LU�ֽⲻ�ܽ�������ʽ�任
Matrix Matrix::niMatrix()
{
    if(row!=col)
    {
        cout<<"Data ni is no "<<endl;
        exit(-1);
    }
    if(this->detMatrix()==0)//�������������ʽ����������Ԫ��ȥ�ı��˲���������δ��ݲ��ı���һ������
    {
        cout<<"Data det is no so ni is no "<<endl;
        exit(-1);
    }
    unsigned int i=0,j=0,k=0;//�������-1���������ض���Ϊint��
    double temp;
    Matrix mCopy=*this;
    Matrix UMatrix=*this;
    Matrix LMatrix=*this;
    Matrix UniMatrix=*this;
    Matrix LniMatrix=*this;
    ColData cda(col);
    Data aij(row,cda);
    for(k=0; k<col-1; k++)//k��ʾ��k����Ԫ��һ����Ҫn-1��
    {
        for(i=k+1; i<row; i++)
        {
            aij[i][k]=data[i][k]/data[k][k];// ��k����Ԫ����Ԫ��Ϊ��k�е�k�У��ѵ�k�����µ��ж�������Ԫ
            for(j=k; j<col; j++)//����k�����µ�ÿһ�е�ÿһ��Ԫ�ض���ȥ��������Ԫ���ӵĳ˻�
            {
                data[i][j]-=aij[i][k]*data[k][j];
            }
        }
    }
    UMatrix=*this;
    for(j=0; j<col; j++)
    {
        for(i=j+1; i<row; i++)
        {
            temp=0;
            for(k=0; k<j; k++)
            {
                temp=LMatrix.data[i][k]*UMatrix.data[k][j];
            }
            LMatrix.data[i][j]=1/UMatrix.data[j][j]*(mCopy.data[i][j]-temp);
        }
    }
    for(i=0; i<row; i++)
    {
        for(j=0; j<col; j++)
        {
            if(i==j)
                LMatrix.data[i][j]=1;
            if(j>i)
                LMatrix.data[i][j]=0;
        }
    }
    Matrix mults;
    mults=*this;
    mults=mults.multsMatrix(LMatrix,UMatrix);
    Matrix LU=mults;
    //cout<<"lu"<<endl;
    //mults.print();

    //����u��
    for(j=0; j<col; j++)
    {
        for(i=j; (int)i>=0; i--)
        {
            if(i==j)
                UniMatrix.data[i][j]=1/UMatrix.data[i][j];
            else
            {
                temp=0;
                for(k=j; k>i; k--)
                {
                    temp+=UMatrix.data[i][k]*UniMatrix.data[k][j];
                }
                UniMatrix.data[i][j]=-1/UMatrix.data[i][i]*temp;
            }
        }
        ///�ؼ���������������
        for(i=j+1; i<row; i++)
            UniMatrix.data[i][j]=0;
    }
    //����l��
    for(j=0; j<col; j++)
    {
        for(i=0; i<row; i++)
        {
            if(j==i)
                LniMatrix.data[i][j]=1;
            else
            {
                temp=0;
                for(k=j; k<i; k++)
                {
                    temp+=(LMatrix.data[i][k]*LniMatrix.data[k][j]);
                }
                LniMatrix.data[i][j]=-temp;
            }
        }
    }

    mults=mults.multsMatrix(UniMatrix,LniMatrix);
    *this=mCopy;
    Matrix I=*this;
    I=I.multsMatrix(LU,mults);
    //cout<<"LU"<<"*"<<"LUni"<<endl;
    //I.print();
    return mults;
}
/*unsigned int LDL(Data x)//�����LDL�ֽ⣬��֪���������ھ�������ֵ�������������
{
    Data l;
    l.initData(&l,x.col,x.row);
    Data d;
    d.initData(&d,x.col,x.row);
    unsigned int i,j,k;
    Data temp;
    temp.initData(&temp,x.col,x.row);
    for(i=0;i<x.col;i++)
    {
        l.mat[i][i]=1;
        for(j=0;j<i;j++)
        {
            for(k=0;k<j;k++)
            {
                temp.mat[i][k]=l.mat[i][k]*d.mat[k][k];
                temp.mat[i][j]-=temp.mat[i][k]*l.mat[j][k];
            }
            temp.mat[i][j]=temp.mat[i][j]+x.mat[i][j];
            l.mat[i][j]=temp.mat[i][j]/d.mat[j][j];
        }
        d.mat[i][i]=x.mat[i][i];
        for(k=0;k<i;k++)
        {
            d.mat[i][i]-=temp.mat[i][k]*l.mat[i][k];
        }
    }
    for(i=0;i<x.col;i++)
    {
        for(j=0;j<x.row;j++)
        {
            cout<<l.mat[i][j]<<"  ";
        }
        cout<<endl;
    }
    for(i=0;i<x.col;i++)
    {
        for(j=0;j<x.row;j++)
        {
            cout<<d.mat[i][j]<<"  ";
        }
        cout<<endl;
    }
}*/

