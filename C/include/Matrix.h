#ifndef Matrix_H
#define Matrix_H
#include "LoadData.h"
using namespace std;
class Matrix
{
public:
    Data data;
    unsigned int row;
    unsigned int col;
public:
    Matrix();

    Matrix(unsigned int row, unsigned int col,float lamd, const char* type);

    void initMatrix(unsigned int row, unsigned int col,float lamd, const char* type);

    void LoadData(const char *filename);

    void print();

    Matrix copyMatrix();

    Matrix getOneRow(unsigned int iRow);

    Matrix getOneCol(unsigned int jCol);

    void deleteOneRow(unsigned int iRow);

    void deleteOneCol(unsigned int iCol);

    Matrix transposeMatrix();//������ʽ��ת��

    Matrix addMatrix(const Matrix &matrix1,const Matrix &matrix2);

    Matrix subMatrix(const Matrix &matrix1,const Matrix &matrix2);

    Matrix multsMatrix(Matrix matrix1,Matrix matrix2);//������ʽ�����

    Matrix dotmultsMatrix(Matrix matrix1,Matrix matrix2);//������ʽ�����

    double detMatrix();//����ʽ

    Matrix niMatrix();//��˹��Ԫ��������,�ر�ע�⣬LU�ֽⲻ�ܽ�������ʽ�任
};
#endif // Data_H
