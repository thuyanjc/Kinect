#pragma once

#define WIN32_LEAN_AND_MEAN

//#pragma comment(lib, ".\\lib\\bluetooth1.lib" )

#include <Kinect.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <Python.h>
#include "KinectJointFilter.h"
//#include "bluetooth1.h"
//#include "Menu.h"

#define ROBOTARM 1


template<class Interface>
inline void SafeRelease(Interface*& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}



class CBodyBasics
{
		//kinect 2.0 ����ȿռ�ĸ�*���� 424 * 512���ڹ�������˵��
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

public:
	CBodyBasics();
	~CBodyBasics();
	void KinectStart(int option); //��kinect�����ĺ���
	int Update(int option);//��ùǼܡ�������ֵͼ�������Ϣ
	void InitializeDefaultSensor();//���ڳ�ʼ��kinect

	//ͨ����õ�����Ϣ���ѹǼܺͱ�����ֵͼ������
	int ProcessBody(int nBodyCount, IBody** ppBodies, int option);

	//�赸ģʽ
	void ProcessDance(Joint joints[]);

	//������ģʽ
	void ProcessSensor(Joint joints[]);

	//�ж���Ƶ��Դ���kinect2�ķ��򣬷�ΧΪ-50~+50�����ң�
	float ProcessAudio();

	//����ָ����������״̬λ
	int statusControl(Joint Vertex, Joint Vertex1, Joint Vertex2);

	static void* KinectThread(void* para);
	void DetectionControl();
	void RunKinect();

	void TempHandle();
	unsigned char Dist[108];

	static DWORD  framenumber;			//����֡���
	float spinemid_xin;		//����
	float spinemid_yin;
	float spinemid_zin;
	float spinemid_xout;
	float spinemid_yout;
	float spinemid_zout;
	float spinemid_x;
	float spinemid_y;
	float spinemid_z;
	float spinebase_yin, spinebase_yout, spinebase_y;
	float rightfoot_yin = 0, rightfoot_yout = 0, rightfoot_y = 0;
	float rightAnkle_yin = 0, rightAnkle_yout = 0, rightAnkle_y = 0;
	float leftfoot_yin = 0, leftfoot_yout = 0, leftfoot_y = 0;
	float base_foot_in = 0, base_foot_out = 0, base_foot = 0;
	float spinetemp = 0;

	const double thresh_x = 0.15;
	const double thresh_z = 0.15;
	const double handdistance = 0.10;
	unsigned char Msg[10];
	char Msgsend[10];


	int flag = 0;		//�¶ױ�־λ
	int handstate = 0;

	int armstate = 1; //1 for start

	char detectionstate = 0;

	bool Is_SendMsg = true;
	bool Is_SendPort = false;
	int close = 0;
	int Humiclose = 0;
	char TempData[4];

	static CBodyBasics* CurBodyBasics;
	//pthread_t threads_kinect;
	int rc_kinect;
	//pthread_t threads_humi;
	int rc_humi;
private:
	CBodyBasics* BodyBasicPtr;
	IKinectSensor* m_pKinectSensor;//kinectԴ
	ICoordinateMapper* m_pCoordinateMapper;//��������任
	IBodyFrameReader* m_pBodyFrameReader;//���ڹǼ����ݶ�ȡ
	IDepthFrameReader* m_pDepthFrameReader;//����������ݶ�ȡ
	IBodyIndexFrameReader* m_pBodyIndexFrameReader;//���ڱ�����ֵͼ��ȡ


	void Detection(Joint joints[]);
	double  Distance(Joint p1, Joint p2);

	void AngleHandle(Joint joints[]);
	int ElbowCalc(Joint Vertex, Joint Vertex1, Joint Vertex2);
	int HandCalc(Joint Vertex1, Joint Vertex2, Joint Vertex3);
	int ChestCalc(Joint Vertex1, Joint Vertex2, Joint Vertex3);
	void ShoulderCalc(Joint Vertex1, Joint Vertex2, int* Address);
};