#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<io.h>
#include<time.h>
#include<windows.h>
#include <process.h>
#include<mmsystem.h>
#include "Menu.h"
#pragma comment(lib, "winmm.lib")

void drawMain();        //���������� 
void drawMenu();      	//���Ʋ˵�
void drawLine();        //����menu����ķָ���
void drawGameMenu();    //������Ϸ�˵����� 
void produceGameArray();  //������Ϸ������ 
void drawGameArea();      //������Ϸ���� 

void drawSettingDiag();   //������Ϸ���ý��� 
void drawRankDiag();       //�������а���� 
void drawHelpDiag();       //���ư������� 

void mainMouseHit();       //����������� 

void settingMouseHit();    //��Ϸ��������� 
void rankMouseHit();       //���а��������� 
void helpMouseHit();       //��Ϸ������������� 


void gamePlaying();   //����Ϸ 
void gameOver();        //��Ϸ���� 

void drawMain()//����ӭ�˵�
{

	// �ñ���ɫ�����Ļ
	cleardevice();
	loadimage(NULL, _T(".\\pic\\background.jpeg"), 800, 600, true);

	setbkmode(TRANSPARENT);

	IMAGE tmpImage;
	loadimage(&tmpImage, _T(".\\pic\\welcome.jpg"), 540, 540, true);
	putimage(210, 50, &tmpImage);



	/*
	//��Ϸ���ڽ���ʱ�����Ե����ʼ��Ϸ��ť
	IMAGE tmpImage1;
	loadimage(&tmpImage1, _T("pic\\button\\startgame0.jpg"), 100, 40, true);
	putimage(50, 50, &tmpImage1);
	*/

}