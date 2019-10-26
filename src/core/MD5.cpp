#include <iostream>
#include "core/MD5.h"

/*4����㺯��*/
inline unsigned int F(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Y) | ((~X) & Z);
}

inline unsigned int G(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Z) | (Y & (~Z));
}

inline unsigned int H(unsigned int X, unsigned int Y, unsigned int Z) {
    return X ^ Y ^ Z;
}

inline unsigned int I(unsigned int X, unsigned int Y, unsigned int Z) {
    return Y ^ (X | (~Z));
}
/*4����㺯������*/

/*32λ��ѭ������ʵ�ֺ���*/
void ROL(unsigned int &s, unsigned short cx) {
    if (cx > 32)cx %= 32;
    s = (s << cx) | (s >> (32 - cx));
    return;
}

/*B\L��ת������UINT����*/
void ltob(unsigned int &i) {
    unsigned int tmp = i;//���渱��
    byte *psour = (byte * ) & tmp, *pdes = (byte * ) & i;
    pdes += 3;//����ָ�룬׼�����ҵ�ת
    for (short i = 3; i >= 0; --i) {
        CopyMemory(pdes - i, psour + i, 1);
    }
    return;
}

/*
MD5ѭ�����㺯����label=�ڼ���ѭ����1<=label<=4����lGroup����=4�����Ӹ�����M=���ݣ�16��32λ��ָ�룩
�����������з�ʽ: --A--D--C--B--���� lGroup[0]=A; lGroup[1]=D; lGroup[2]=C; lGroup[3]=B;
*/
void AccLoop(unsigned short label, unsigned int *lGroup, void *M) {
    unsigned int *i1, *i2, *i3, *i4, TAcc, tmpi = 0; //����:4��ָ�룻 T���ۼ����� �ֲ�����
    typedef unsigned int(*clac)(unsigned int X, unsigned int Y, unsigned int Z); //���庯������
    const unsigned int rolarray[4][4] = {
            {7, 12, 17, 22},
            {5, 9,  14, 20},
            {4, 11, 16, 23},
            {6, 10, 15, 21}
    };//ѭ������-λ����
    const unsigned short mN[4][16] = {
            {0, 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15},
            {1, 6, 11, 0,  5,  10, 15, 4,  9,  14, 3,  8,  13, 2,  7,  12},
            {5, 8, 11, 14, 1,  4,  7,  10, 13, 0,  3,  6,  9,  12, 15, 2},
            {0, 7, 14, 5,  12, 3,  10, 1,  8,  15, 6,  13, 4,  11, 2,  9}
    };//���������
    const unsigned int *pM = static_cast<unsigned int *>(M);//ת������Ϊ32λ��Uint
    TAcc = ((label - 1) * 16) + 1; //���ݵڼ���ѭ����ʼ��T���ۼ���
    clac clacArr[4] = {F, G, H, I}; //���岢��ʼ�����㺯��ָ������

    /*һ��ѭ����ʼ��16��->16�Σ�*/
    for (short i = 0; i < 16; ++i) {
        /*����ָ���Ա任*/
        i1 = lGroup + ((0 + i) % 4);
        i2 = lGroup + ((3 + i) % 4);
        i3 = lGroup + ((2 + i) % 4);
        i4 = lGroup + ((1 + i) % 4);

        /*��һ�����㿪ʼ: A+F(B,C,D)+M[i]+T[i+1] ע:��һ����ֱ�Ӽ���T��*/
        tmpi = (*i1 + clacArr[label - 1](*i2, *i3, *i4) + pM[(mN[label - 1][i])] +
                (unsigned int) (0x100000000UL * abs(sin((double) (TAcc + i)))));
        ROL(tmpi, rolarray[label - 1][i % 4]);//�ڶ���:ѭ������
        *i1 = *i2 + tmpi;//������:��Ӳ���ֵ������
    }
    return;
}

/*�ӿں�������ִ���������*/
unsigned int *MD5(const char *mStr) {
    unsigned int mLen = strlen(mStr); //�����ַ�������
    if (mLen < 0) return 0;
    unsigned int FillSize = 448 - ((mLen * 8) % 512); //����������bit��
    unsigned int FSbyte = FillSize / 8; //���ֽڱ�ʾ�������
    unsigned int BuffLen = mLen + 8 + FSbyte; //���������Ȼ���˵����ĳ���
    unsigned char *md5Buff = new unsigned char[BuffLen]; //���仺����
    CopyMemory(md5Buff, mStr, mLen); //�����ַ�����������

    /*������俪ʼ*/
    md5Buff[mLen] = 0x80; //��һ��bit���1
    ZeroMemory(&md5Buff[mLen + 1], FSbyte - 1); //����bit���0����һ���ú���ΪFillMemory
    unsigned long long lenBit = mLen * 8ULL; //�����ַ������ȣ�׼�����
    CopyMemory(&md5Buff[mLen + FSbyte], &lenBit, 8); //��䳤��
    /*����������*/

    /*���㿪ʼ*/
    unsigned int LoopNumber = BuffLen / 64; //��16����Ϊһ���飬�����������
    unsigned int A = 0x67452301, B = 0x0EFCDAB89, C = 0x98BADCFE, D = 0x10325476;//��ʼ4�����ӣ�С������
    unsigned int *lGroup = new unsigned int[4]{A, D, C, B}; //���Ӹ�������,����Ϊ����ֵ����
    for (unsigned int Bcount = 0; Bcount < LoopNumber; ++Bcount) //�����ѭ����ʼ
    {
        /*����4�μ����Сѭ��*/
        for (unsigned short Lcount = 0; Lcount < 4;) {
            AccLoop(++Lcount, lGroup, &md5Buff[Bcount * 64]);
        }
        /*���������Ϊ��һ�ֵ����ӻ����������*/
        A = (lGroup[0] += A);
        B = (lGroup[3] += B);
        C = (lGroup[2] += C);
        D = (lGroup[1] += D);
    }
    /*ת���ڴ��еĲ��ֺ����������ʾ*/
    ltob(lGroup[0]);
    ltob(lGroup[1]);
    ltob(lGroup[2]);
    ltob(lGroup[3]);
    delete[] md5Buff; //����ڴ沢����
    return lGroup;
}


string calcMD5(string rawPassword) {
    void ROL(unsigned int &s, unsigned short cx); //32λ��ѭ������ʵ�ֺ���
    void ltob(unsigned int &i); //B\L��ת������UINT����
    unsigned int *MD5(const char *mStr); //�ӿں�������ִ��������䣬����MD5ʱ���ô˺���
    char tmpstr[256], buf[4][10];
    unsigned int *tmpGroup = MD5(rawPassword.c_str());
    sprintf_s(buf[0], "%8X", tmpGroup[0]);
    sprintf_s(buf[1], "%8X", tmpGroup[3]);
    sprintf_s(buf[2], "%8X", tmpGroup[2]);
    sprintf_s(buf[3], "%8X", tmpGroup[1]);
    delete[] tmpGroup;
    int k = 0;
    string result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 8; j++) {
            result += buf[i][j];
            //result[k] = buf[i][j];
            //k++;
        }
    return result;
}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
