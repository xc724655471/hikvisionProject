#include "stdafx.h"
#include "HIK.h"


HIK::HIK()
{


	//NET_DVR_SetSDKInitCfg(1,);

	//// ��ʼ��
	//NET_DVR_Init();
}


HIK::~HIK()
{

	//ע���û�
	if(lUserID>0)
		NET_DVR_Logout(lUserID);
	//�ͷ�SDK��Դ
	NET_DVR_Cleanup();
}

void HIK::Init()
{
	NET_DVR_INIT_CFG_ABILITY * tt;
	NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_ABILITY, (void*)tt);

	// ��ʼ��
	bool i=NET_DVR_Init();
	cout << i << endl;


	//NET_DVR_SetConnectTime(2000, 1);
	//NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//�����쳣��Ϣ�ص�����
	//NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//---------------------------------------
	// ��ȡ����̨���ھ��
	//HMODULE hKernel32 = GetModuleHandle(_T("kernel32"));
	//GetConsoleWindowAPI = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");


	//��¼�����������豸��ַ����¼�û��������
//	NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
	struLoginInfo.bUseAsynLogin = 0; //ͬ����¼��ʽ
	strcpy_s(struLoginInfo.sDeviceAddress, "192.168.0.252"); //�豸IP��ַ
	struLoginInfo.wPort = 8000; //�豸����˿�
	strcpy_s(struLoginInfo.sUserName, "admin"); //�豸��¼�û���
	strcpy_s(struLoginInfo.sPassword, "Changeme"); //�豸��¼����

												 //�豸��Ϣ, �������
//	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };

	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (lUserID < 0)
	{
		printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}




}

bool HIK::GetCompressionParameters()
{

	if (lUserID < 0)
	{
		return false;
	}
	int iRet;
	long ch = 0;
	//��ȡѹ������
	DWORD dwReturnLen;
	NET_DVR_NETCFG_V30 struParams;
	iRet = NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_NETCFG_V50, 0,
		&struParams, sizeof(NET_DVR_NETCFG_V30), &dwReturnLen);


	
	if (!iRet)
	{
		//DWORD error = NET_DVR_GetLastError();
		cout <<"a"<< NET_DVR_GetLastError()<<"a" << endl;
		cout << iRet << endl;
		printf("NET_DVR_GetDVRConfig NET_DVR_GET_COMPRESSCFG_V30 error.\n");
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return false;
	}
	else
	{
		cout << "Video Bitrate is" << struParams.dwSize << endl;
	}

	return true;
}

bool HIK::GetBasicParameters()
{
	if (lUserID < 0)
	{
		return false;
	}
	DWORD dwReturned = 0;
	memset(&m_struDeviceCfg, 0, sizeof(NET_DVR_DEVICECFG_V40));

	//memset(&m_struDeviceCfg, 0, sizeof(NET_DVR_DEVICECFG));


	if (!NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_DEVICECFG_V40, 0, &m_struDeviceCfg, sizeof(NET_DVR_DEVICECFG_V40), &dwReturned))
	{
		cout << "��ȡ�豸�������ò���ʧ��!" << NET_DVR_GetLastError ()<< endl;
	}
	cout<<m_struDeviceCfg.byDiskNum<<endl;//Ӳ����
	//m_iAlarmInNum = m_struDeviceCfg.byAlarmInPortNum;//����������
	//m_iAlarmOutNum = m_struDeviceCfg.byAlarmOutPortNum;//���������
	//m_iChanNum = m_struDeviceCfg.byChanNum;//ͨ����
	//m_iYKQID = m_struDeviceCfg.dwDVRID;//ң����ID

	return false;
}

bool HIK::EndPreview()
{
	//�ر�Ԥ��
	NET_DVR_StopRealPlay(lRealPlayHandle);

	return false;
}

bool HIK::StartPreview()
{
	//����Ԥ�������ûص�������
	//LONG lRealPlayHandle;
	//HWND hWnd = GetConsoleWindowAPI();     //��ȡ���ھ��
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	//struPlayInfo.hPlayWnd = hWnd;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	struPlayInfo.lChannel = 1;       //Ԥ��ͨ����
	struPlayInfo.dwStreamType = 0;       //0-��������1-��������2-����3��3-����4���Դ�����
	struPlayInfo.dwLinkMode = 0;       //0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP
	struPlayInfo.bBlocked = 1;       //0- ������ȡ����1- ����ȡ��

	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
	if (lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return false;
	}

	Sleep(10000);
	//return false;
}
