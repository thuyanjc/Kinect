#include "KinectMain.h"
#include "Python.h"

using namespace std;
//using   namespace   cv;
using   namespace   Sample;

#define PI 3.141592653

CBodyBasics* CBodyBasics::CurBodyBasics = nullptr;

//���캯��
CBodyBasics::CBodyBasics() :
	m_pKinectSensor(NULL),
	m_pCoordinateMapper(NULL),
	m_pBodyFrameReader(NULL),
	m_pDepthFrameReader(NULL),
	m_pBodyIndexFrameReader(NULL)
{
	//pDlg = new CMFC_DEMO01Dlg();
}

// ��������
CBodyBasics::~CBodyBasics()
{
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);
	SafeRelease(m_pDepthFrameReader);
	SafeRelease(m_pBodyIndexFrameReader);

	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
	SafeRelease(m_pKinectSensor);
}


//��һ�㣬��Kinect
void CBodyBasics::KinectStart(int option)
{
	
	
	Py_Initialize();
	PyRun_SimpleString("print('Please say start!')\n");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyRun_SimpleString("import speechrec");
	PyRun_SimpleString("speechrec.speech_rec()");
	Py_Finalize();

	

	ofstream outfile(".\\angles.txt");
	outfile << "*";


	int jiaoyan = 0;
	for (int i = 0; i < 10; i++) {
		Msg[i] = 255;
	}
	for (int i = 1; i < 8; i++) {
		jiaoyan += (int)Msg[i];
		cout << (int)Msg[i] << endl;
		outfile << "#" << (int)Msg[i];
	}

	//״̬λstatus
	outfile << "#" << 3;

	//��żУ��λ
	jiaoyan += 3;
	jiaoyan = jiaoyan % 2;
	outfile << "#" << jiaoyan;

	outfile.close();



	Py_Initialize();

	PyRun_SimpleString("print('Program is starting...')\n");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyRun_SimpleString("import main");
	PyRun_SimpleString("main.serial_comm()");

	Py_Finalize();


	CBodyBasics* BodyBasic = new CBodyBasics();
	cout << "Kinect start!" << endl;
	if (option == ROBOTARM) {
		//����ڶ���
		BodyBasic->RunKinect();
	}  
}


/*�ڶ��㣬����Kinect*/
void CBodyBasics::RunKinect() {
	this->close = 0;
	CBodyBasics* BodyBasics = new CBodyBasics();
	cout << "Kinect is running!" << endl;

	/*��ʼ��Msgsend*/
	if (BodyBasics->Is_SendMsg) {
		//BodyBasics->SendMsg.Initialize();
		for (int i = 0; i < 10; i++) {
			BodyBasics->Msgsend[i] = 127;
		}
		BodyBasics->Msgsend[0] = 10;
		//BodyBasics->SendMsg.Send_msg(BodyBasics->Msgsend);
	}
	/*��ʼ��sensor*/
	BodyBasics->InitializeDefaultSensor();

	cout << "Kienct sensor has been initialized!" << endl;
	while (1) {
		//BodyBasics->DetectionControl();
		int status = 2;

		//�жϽ�������ģʽ����ģ��ģʽ
		cout << "��ѡ��ģʽ��" << endl;
		cout << "1. ��������ģʽ  2.����ģʽ  3.������ģʽ" << endl;

		int option = 1;
		cin >> option;
		
		/*
		while (1) {
			BodyBasics->Update(option);
			Sleep(1000);
		}
		*/
		

		
		while (1) {
			int isBreak = BodyBasics->Update(option);
			//cout << "Updating data..." << endl; 
			if (isBreak) {
				break;
			}
			Sleep(8000);
		}
		
		
		//_sleep(1);
		Sleep(1000);

		if (BodyBasics->close)
		{
			break;
		}
	}
}

/*��ʼ��sensor*/
void CBodyBasics::InitializeDefaultSensor()
{
	//����kinect
	GetDefaultKinectSensor(&m_pKinectSensor);

	//�ҵ�kinect�豸
	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;//��ȡ�Ǽ�
		IDepthFrameSource* pDepthFrameSource = NULL;//��ȡ�����Ϣ
		IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;//��ȡ������ֵͼ

		//��kinect
		m_pKinectSensor->Open();
		cout << "kinect is open!" << endl;

		//ProcessAudio();


		//coordinatemapper
		m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);

		//bodyframe    ÿ��ͼ����ͨ��source��reader��frame������
		m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);

		pBodyFrameSource->OpenReader(&m_pBodyFrameReader);

		//depth frame
		m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		pDepthFrameSource->OpenReader(&m_pDepthFrameReader);


		//body index frame
		m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);

		pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);

		SafeRelease(pBodyFrameSource);
		SafeRelease(pDepthFrameSource);
		SafeRelease(pBodyIndexFrameSource);
	}
	/*
	//skeletonImg,���ڻ��Ǽܡ�������ֵͼ��MAT
	skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	skeletonImg.setTo(0);

	//depthImg,���ڻ������Ϣ��MAT
	depthImg.create(cDepthHeight, cDepthWidth, CV_8UC1);
	depthImg.setTo(0);
	*/
}


/*�����㣬ʵʱ���ºʹ�������*/
/// Main processing function
int CBodyBasics::Update(int option)
{

	//�����ʧ��kinect���򲻼�������
	if (!m_pBodyFrameReader) {
		return 1;
	}

	IBodyFrame* pBodyFrame = NULL;//�Ǽ���Ϣ
	IDepthFrame* pDepthFrame = NULL;//�����Ϣ
	IBodyIndexFrame* pBodyIndexFrame = NULL;//������ֵͼ

	//��¼ÿ�β����ĳɹ����
	HRESULT hr = S_OK;

	//-----------------------------��ȡ�Ǽܲ���ʾ----------------------------
	if (SUCCEEDED(hr))
	{
		hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);//��ȡ�Ǽ���Ϣ
	}
	if (SUCCEEDED(hr))
	{
		cout << "ok" << endl;
		IBody* ppBodies[BODY_COUNT] = { 0 };//ÿһ��IBody����׷��һ���ˣ��ܹ�����׷��������

		if (SUCCEEDED(hr))
		{
			//��kinect׷�ٵ����˵���Ϣ���ֱ�浽ÿһ��IBody��
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr) )
		{
			//��ÿһ��IBody�������ҵ����ĹǼ���Ϣ�����һ�����
			cout << "processing body, option = " << option << endl;
			//ProcessBody(BODY_COUNT, ppBodies, option);
			//�ж��Ƿ�Ҫ����ѭ��
			int isBreak = ProcessBody(BODY_COUNT, ppBodies, option);
			for (int i = 0; i < _countof(ppBodies); ++i)
			{
				SafeRelease(ppBodies[i]);//�ͷ�����
			}
			SafeRelease(pBodyFrame);//����Ҫ�ͷţ�����֮���޷�����µ�frame����
			return isBreak;
		}
	}
	//SafeRelease(pBodyFrame);//����Ҫ�ͷţ�����֮���޷�����µ�frame����
	return 1;
}

/*���Ĳ㣬���������Ϣ*/
int CBodyBasics::ProcessBody(int nBodyCount, IBody** ppBodies, int option)
{
	//��¼��������Ƿ�ɹ�
	HRESULT hr;
	FilterDoubleExponential filterKinect;	//������˫ָ���˲���

	//����ÿһ��IBody
	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
		if (pBody) {
			BOOLEAN bTracked = false;
			hr = pBody->get_IsTracked(&bTracked);
			filterKinect.Update(pBody);		//ƽ�����ȶ�����Ǽ�

			if (SUCCEEDED(hr) && bTracked)
			{
				Joint joints[JointType_Count];//�洢�ؽڵ���
				HandState leftHandState = HandState_Unknown;//����״̬
				HandState rightHandState = HandState_Unknown;//����״̬

				//��ȡ������״̬
				pBody->get_HandLeftState(&leftHandState);
				pBody->get_HandRightState(&rightHandState);


				//�������״̬�仯���������ǰ����״̬
				if (rightHandState == HandState_Open ) {
					handstate = 2;
					//cout << "�����ſ�" << endl;
				}
				else if (rightHandState == HandState_Closed) {
					handstate = 3;
					//cout << "���Ʊպ�" << endl;
				}
				else if (rightHandState == HandState_NotTracked) {
					handstate = 1;
				}

				//�洢�������ϵ�еĹؽڵ�λ��
				DepthSpacePoint* depthSpacePosition = new DepthSpacePoint[_countof(joints)];

				//��ùؽڵ���
				hr = pBody->GetJoints(_countof(joints), joints);
				if (SUCCEEDED(hr))
				{
					for (int j = 0; j < _countof(joints); ++j)
					{
						//���ؽڵ���������������ϵ��-1~1��ת���������ϵ��424*512��
						m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
					}
					filterKinect.Update(joints);		//ƽ������ÿ��������

					
					if (option == 1) {
						//������⺯��
						if (joints[JointType_SpineBase].Position.Z > 0.5 && joints[JointType_SpineBase].Position.Z < 3.5) {
							//Detection(joints);

							cout << "���ڻ�ȡ�ؽ���Ϣ" << endl;
							AngleHandle(joints);
							cout << "\n\n\n\n angle handle is completed!" << endl;
							cout << "\n\n hand state: " <<leftHandState << " " << rightHandState << endl;

							if (leftHandState == HandState_Open && rightHandState == HandState_Open) {
								return 1;
							}
							else {
								return 0;
							}
							
						}

						else
							cout << "Ϊ�˼���׼ȷ�ȣ��뾡��վ����Kinect 0.5--3.5 ��֮�䣬лл��" << endl;
					}
					else if (option == 2) {
						cout << "\n\n\n\n angle handle is completed!" << endl;
						cout << "\n\n hand state: " << leftHandState << " " << rightHandState << endl;

						ProcessDance(joints);
						
						if (leftHandState == HandState_Open && rightHandState == HandState_Open) {
							return 1;
						}
						else {
							return 0;
						}
						
					}
					else if (option == 3) {
						cout << "\n\n\n hand state: " << leftHandState << " " << rightHandState << endl;

						ProcessSensor(joints);

						if (leftHandState == HandState_Open && rightHandState == HandState_Open) {
							return 1;
						}
						else {
							return 0;
						}
					}
					
				}
				delete[] depthSpacePosition;
			}
		}
	}
	return 0;
}

/*����㣬����ָ����ʽ�����нǶ�����*/
void CBodyBasics::AngleHandle(Joint joints[]) {
	int AngleElbow, AngleHand, AngleChest;
	int AngleSholder[2];
	if (armstate == 1) {
		//�����һ�µ�ǰ�����ĽǶȺ����Ŷ�
		float fAngle = ProcessAudio();
		/*��ʼ��Msg*/
		for (int i = 0; i < 10; i++) {
			Msg[i] = 255;
		}

		int status = 2;

			if (joints[JointType_ElbowRight].Position.X - joints[JointType_ShoulderRight].Position.X < 0.05) {
				cout << "�뽫�ֱ۷��������ⲿ" << endl;
				return;
			}


			if (handstate == 2 || handstate == 3) {

				AngleElbow = ElbowCalc(joints[JointType_ElbowRight], joints[JointType_ShoulderRight], joints[JointType_WristRight]);
				AngleHand = HandCalc(joints[JointType_WristRight], joints[JointType_ThumbRight], joints[JointType_HandTipRight]);
				ShoulderCalc(joints[JointType_ElbowRight], joints[JointType_ShoulderRight], AngleSholder);
				AngleChest = ChestCalc(joints[JointType_HipRight], joints[JointType_HipLeft], joints[JointType_SpineShoulder]);



				if (handstate == 2) {

					Msg[0] = 1;
					Msg[1] = fAngle;
					Msg[2] = AngleSholder[0];
					Msg[3] = AngleSholder[1];
					Msg[4] = AngleElbow;
					Msg[5] = AngleElbow;
					Msg[6] = AngleHand;
					Msg[7] = 255;
				}
				else if (handstate == 3) {
					Msg[0] = 2;
					Msg[1] = fAngle;
					Msg[2] = AngleSholder[0];
					Msg[3] = AngleSholder[1];
					Msg[4] = AngleElbow;
					Msg[5] = AngleElbow;
					Msg[6] = AngleHand;
					Msg[7] = 1;
				}
			}
		

		//��һ��״̬+�߸��Ƕȴ���txt
		ofstream outfile(".\\angles.txt");
		outfile << "*";

		int jiaoyan = 0;
		for (int i = 1; i < 8; i++) {
			Msgsend[i] = (char)Msg[i];
			jiaoyan += (int)Msg[i];
			cout << (int)Msg[i] << endl;
			outfile << "#" << (int)Msg[i] ;
		}
		
		//״̬λstatus
		outfile << "#" << status;

		//��żУ��λ
		jiaoyan += status;
		jiaoyan = jiaoyan % 2;
		outfile << "#" << jiaoyan;
 
		outfile.close();
		

		Py_Initialize();
		PyRun_SimpleString("print('Hello!')\n");

		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append('./')");
		PyRun_SimpleString("import main");
		PyRun_SimpleString("main.serial_comm()");

		Py_Finalize();
		
		Sleep(2000);
		

		//system(".\\bluetooth.exe");
		
		/*
		if (Is_SendMsg && mapstate == 0) {
			SendMsg.Send_msg(Msgsend);
			inttohex(Msg, 10);
		}
		*/
	}
}




float CBodyBasics::ProcessAudio() {
	// ��Ƶ���ݻ�ȡ  
	IAudioSource* audios = nullptr;
	HRESULT hr = m_pKinectSensor->get_AudioSource(&audios);
	IAudioBeam* audiobm = nullptr;
	IAudioBeamList* audiobml = nullptr;
	audios->get_AudioBeams(&audiobml);
	audiobml->OpenAudioBeam(0, &audiobm); // Ŀǰֻ֧�ֵ�һ��  

	float fAngle = 0.0f;
	float fAngleConfidence = 0.0f;


	//Mat img(200, 400, CV_8UC3, Scalar(255, 255, 255));
	//	circle(img, Point(200, 0), 100, Scalar(0, 0, 0));
		fAngle = 0.0f;
		fAngleConfidence = 0.0f;
		audiobm->get_BeamAngle(&fAngle); // ��ȡ��Ƶ�ĽǶ�
		audiobm->get_BeamAngleConfidence(&fAngleConfidence); // ��ȡ��Ƶ�Ŀ��Ŷȣ�0 - 1��  
		fAngle = (fAngle / 3.1415926f)*180.0f;
		fAngle = (fAngle + 50.0f) * 255.0f / 100.0f;
		printf("Angle: %3.2f  Angle Confidence: (%1.2f)\n", fAngle, fAngleConfidence);
		//������ͼ(img, fAngle, fAngleConfidence);
		//imshow("", img);
		//Sleep(1000);
		return fAngle;
}

void CBodyBasics::ProcessDance(Joint joints[]) {
	int status = 2;
	status = statusControl(joints[JointType_WristRight], joints[JointType_WristLeft], joints[JointType_Head]);
	if (status == 6 || status == 7) {
		status = 2;
	}

	/*��ʼ��Msg*/
	for (int i = 0; i < 10; i++) {
		Msg[i] = 255;
	}


	//��һ��״̬+�߸��Ƕȴ���txt
	ofstream outfile(".\\angles.txt");
	outfile << "*";

	int jiaoyan = 0;
	for (int i = 1; i < 8; i++) {
		jiaoyan += (int)Msg[i];
		cout << (int)Msg[i] << endl;
		outfile << "#" << (int)Msg[i];
	}

	//״̬λstatus
	outfile << "#" << status;

	//��żУ��λ
	jiaoyan += status;
	jiaoyan = jiaoyan % 2;
	outfile << "#" << jiaoyan;

	outfile.close();

	cout << "dancing status: " << status << endl;


	Py_Initialize();
	PyRun_SimpleString("print('Hello, the robot is dancing!')\n");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyRun_SimpleString("import main");
	PyRun_SimpleString("main.serial_comm()");

	Py_Finalize();

	Sleep(2000);


}

void CBodyBasics::ProcessSensor(Joint joints[]) {
	int status = 2;
	status = statusControl(joints[JointType_WristRight], joints[JointType_WristLeft], joints[JointType_Head]);
	if (status == 5) {
		status = 2;
	}

	/*��ʼ��Msg*/
	for (int i = 0; i < 10; i++) {
		Msg[i] = 255;
	}


	//��һ��״̬+�߸��Ƕȴ���txt
	ofstream outfile(".\\angles.txt");
	outfile << "*";

	int jiaoyan = 0;
	for (int i = 1; i < 8; i++) {
		jiaoyan += (int)Msg[i];
		cout << (int)Msg[i] << endl;
		outfile << "#" << (int)Msg[i];
	}

	//״̬λstatus
	outfile << "#" << status;

	//��żУ��λ
	jiaoyan += status;
	jiaoyan = jiaoyan % 2;
	outfile << "#" << jiaoyan;

	outfile.close();

	cout << "sensor status: " << status << endl;


	Py_Initialize();
	PyRun_SimpleString("print('Hello, the sensors are working!')\n");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyRun_SimpleString("import main");
	PyRun_SimpleString("main.serial_comm()");

	Py_Finalize();

	Sleep(2000);


}

int CBodyBasics::statusControl(Joint WristRight, Joint WristLeft, Joint Head) {
	int status = 2;
	//˫��չ��
	if (Distance(WristLeft, WristRight) > 1) {
		status = 9;
	}
	//���־ٹ�ͷ������ʼ�赸
	if (WristLeft.Position.Y > Head.Position.Y && WristRight.Position.Y < Head.Position.Y) {
		status = 5;
	}
	//���־ٹ�ͷ��������
	if (WristRight.Position.Y > Head.Position.Y && WristLeft.Position.Y < Head.Position.Y) {
		status = 6;
	}
	//˫�־ٹ�ͷ������ʪ��
	if (WristLeft.Position.Y > Head.Position.Y && WristRight.Position.Y > Head.Position.Y) {
		status = 7;
	}
	return status;
}



//��2��������֮��ľ���
double CBodyBasics::Distance(Joint p1, Joint p2)
{
	return sqrt(pow(p2.Position.X - p1.Position.X, 2) + pow(p2.Position.Y - p1.Position.Y, 2) + pow(p2.Position.Z - p1.Position.Z, 2));
}



/*�����ؽڽǶȵļ���*/
int CBodyBasics::ElbowCalc(Joint ElbowRight, Joint ShoulderRight, Joint WristRight) {
	float vector1[3], vector2[3];
	float dotresult, length1, length2;
	float cosine, angle;
	vector1[0] = ShoulderRight.Position.X - ElbowRight.Position.X;
	vector1[1] = ShoulderRight.Position.Y - ElbowRight.Position.Y;
	vector1[2] = ShoulderRight.Position.Z - ElbowRight.Position.Z;
	vector2[0] = WristRight.Position.X - ElbowRight.Position.X;
	vector2[1] = WristRight.Position.Y - ElbowRight.Position.Y;
	vector2[2] = WristRight.Position.Z - ElbowRight.Position.Z;
	dotresult = vector1[0] * vector2[0] + vector1[1] * vector2[1] + vector1[2] * vector2[2];
	length1 = sqrt(pow(vector1[0], 2) + pow(vector1[1], 2) + pow(vector2[2], 2));
	length2 = sqrt(pow(vector2[0], 2) + pow(vector2[1], 2) + pow(vector2[2], 2));
	cosine = dotresult / (length1 * length2);
	angle = acos(cosine) * 180.0 / PI;
	if (angle < 20) {
		angle = 20;
	}
	if (angle > 160) {
		angle = 160;
	}
	angle = angle / 2.0;
	angle = angle * (254.0 / 70.0) - 35.285714;
	int intangle = (int)angle;
	if (intangle < 1) {
		intangle = 1;
	}
	if (intangle > 254) {
		intangle = 254;
	}
	intangle = -intangle + 256;

	return intangle;
}

int CBodyBasics::HandCalc(Joint Vertex1, Joint Vertex2, Joint Vertex3) {
	float edge1[3], edge2[3];
	edge1[0] = Vertex2.Position.X - Vertex1.Position.X;
	edge1[1] = Vertex2.Position.Y - Vertex1.Position.Y;
	edge1[2] = Vertex2.Position.Z - Vertex1.Position.Z;
	edge2[0] = Vertex3.Position.X - Vertex1.Position.X;
	edge2[1] = Vertex3.Position.Y - Vertex1.Position.Y;
	edge2[2] = Vertex3.Position.Z - Vertex1.Position.Z;
	float NormalVector[3];
	NormalVector[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
	NormalVector[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
	NormalVector[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
	double length = sqrt(pow(NormalVector[0], 2) + pow(NormalVector[1], 2) + pow(NormalVector[2], 2));
	float UnitNormalVector[3];
	UnitNormalVector[0] = NormalVector[0] / length;
	UnitNormalVector[1] = NormalVector[1] / length;
	UnitNormalVector[2] = NormalVector[2] / length;
	float Yaxis[3] = { 0,1,0 };
	float dotresult;
	float angle;
	dotresult = UnitNormalVector[0] * Yaxis[0] + UnitNormalVector[1] * Yaxis[1] + UnitNormalVector[2] * Yaxis[2];
	angle = acos(dotresult) * 180.0 / PI;
	if (angle < 40) {
		angle = 40;
	}
	if (angle > 140) {
		angle = 140;
	}
	angle = angle * (254.0 / 100.0) - 100.6;
	int intangle = int(angle);
	if (intangle < 1) {
		intangle = 1;
	}
	if (intangle > 254) {
		intangle = 254;
	}
	return intangle;
}

int CBodyBasics::ChestCalc(Joint ShoulderRight, Joint ShoulderLeft, Joint SpineMid) {
	float edge1[3], edge2[3];
	edge1[0] = ShoulderLeft.Position.X - ShoulderRight.Position.X;
	edge1[1] = ShoulderLeft.Position.Y - ShoulderRight.Position.Y;
	edge1[2] = ShoulderLeft.Position.Z - ShoulderRight.Position.Z;
	edge2[0] = SpineMid.Position.X - ShoulderRight.Position.X;
	edge2[1] = SpineMid.Position.Y - ShoulderRight.Position.Y;
	edge2[2] = SpineMid.Position.Z - ShoulderRight.Position.Z;
	float NormalVector[3];
	NormalVector[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
	NormalVector[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
	NormalVector[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
	double length = sqrt(pow(NormalVector[0], 2) + pow(NormalVector[1], 2) + pow(NormalVector[2], 2));
	float UnitNormalVector[3];
	UnitNormalVector[0] = NormalVector[0] / length;
	UnitNormalVector[1] = NormalVector[1] / length;
	UnitNormalVector[2] = NormalVector[2] / length;
	float Yaxis[3] = { 1,0,0 };
	float dotresult;
	float angle;
	int intangle;
	dotresult = UnitNormalVector[0] * Yaxis[0] + UnitNormalVector[1] * Yaxis[1] + UnitNormalVector[2] * Yaxis[2];
	angle = acos(dotresult) * 180.0 / PI;
	if (angle > 120) {
		angle = 120;
	}
	if (angle < 60) {
		angle = 60;
	}
	angle = angle * (254.0 / 60.0) - 253.0;
	intangle = (int)angle;
	if (intangle < 1) {
		intangle = 1;
	}
	if (intangle > 254) {
		intangle = 254;
	}
	return intangle;
}

void CBodyBasics::ShoulderCalc(Joint ElbowRight, Joint ShoulderRight, int* AngleShoulder) {
	float UnitVector[3];
	UnitVector[0] = ElbowRight.Position.X - ShoulderRight.Position.X;
	UnitVector[1] = ElbowRight.Position.Y - ShoulderRight.Position.Y;
	UnitVector[2] = ElbowRight.Position.Z - ShoulderRight.Position.Z;
	double length = sqrt(pow(UnitVector[0], 2) + pow(UnitVector[1], 2) + pow(UnitVector[2], 2));
	UnitVector[0] = UnitVector[0] / length;
	UnitVector[1] = UnitVector[1] / length;
	UnitVector[2] = UnitVector[2] / length;
	float SphericalAngle[2];

	SphericalAngle[0] = atan(UnitVector[0] / UnitVector[2]) * 180.0 / PI; //phi
	SphericalAngle[1] = acos(UnitVector[1]) * 180.0 / PI; //theta

	if (SphericalAngle[1] < 20) {
		SphericalAngle[1] = 20;
	}
	if (SphericalAngle[1] > 160) {
		SphericalAngle[1] = 160;
	}
	SphericalAngle[1] = SphericalAngle[1] * (254.0 / 140.0) - 35.285714;

	if (SphericalAngle[0] >= 0) {
		SphericalAngle[0] = SphericalAngle[0];
	}
	else if (SphericalAngle[0] < 0) {
		SphericalAngle[0] = SphericalAngle[0] + 180.0;
	}
	if (SphericalAngle[0] < 70) {
		SphericalAngle[0] = 70;
	}
	if (SphericalAngle[0] > 140) {
		SphericalAngle[0] = 140;
	}
	SphericalAngle[0] = SphericalAngle[0] * (254.0 / 70.0) - 253.0;

	AngleShoulder[0] = (int)SphericalAngle[0];
	AngleShoulder[1] = (int)SphericalAngle[1];



	if (AngleShoulder[0] < 1) {
		AngleShoulder[0] = 1;
	}
	if (AngleShoulder[0] > 254) {
		AngleShoulder[0] = 254;
	}
	if (AngleShoulder[1] < 1) {
		AngleShoulder[1] = 1;
	}
	if (AngleShoulder[1] > 254) {
		AngleShoulder[1] = 254;
	}
}

