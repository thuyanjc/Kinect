#include "stdafx.h"
#include <stdio.h>
#include<time.h>
#include<graphics.h>
#include <signal.h>
//#include "KinectMain.h"

//ȫ�ֱ���
#define leftedge  70         //��Ϸ������߿����
#define topedge   140        //��Ϸ�����ϱ߿����
#define iconNum	  24          //��ϷͼƬ��Ŀ

#define iconX	  67          //��ϷͼƬ�ĳ�
#define iconY	  70          //��ϷͼƬ�Ŀ�
#define iconXNum     6           //������ϷͼƬ����
#define iconYNum     4           //��ϷͼƬ����

#define promtime    2            //��ʾ����
#define shuffletime    1            //��ʾ����

#define countdwontime    360     //����ʱ


extern int imageX;			  //��ϷͼƬ�ĳ�
extern int imageY;            //��ϷͼƬ�Ŀ�

extern MOUSEMSG mouse;

extern int gameGrid[12][14];  //��Ϸͼֽ

extern int timeCount;

typedef struct
{
	int no;
	char name[36];
	IMAGE file;
}image;
extern image iconInfo[iconNum];
extern IMAGE bkImage;         //���ͼƬ


extern int imageX;
extern int imageY;

extern boolean isImageLoad;

extern int yLine;
extern int xLine;

void drawMain();
