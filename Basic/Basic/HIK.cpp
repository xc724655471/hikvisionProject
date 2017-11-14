#include "stdafx.h"
#include "HIK.h"


HIK::HIK()
{


	//NET_DVR_SetSDKInitCfg(1,);

	//// 初始化
	//NET_DVR_Init();
}


HIK::~HIK()
{

	//注销用户
	if(lUserID>0)
		NET_DVR_Logout(lUserID);
	//释放SDK资源
	NET_DVR_Cleanup();
}

void HIK::Init()
{
	NET_DVR_INIT_CFG_ABILITY * tt;
	NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_ABILITY, (void*)tt);

	// 初始化
	bool i=NET_DVR_Init();
	cout << i << endl;


	//NET_DVR_SetConnectTime(2000, 1);
	//NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//设置异常消息回调函数
	//NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//---------------------------------------
	// 获取控制台窗口句柄
	//HMODULE hKernel32 = GetModuleHandle(_T("kernel32"));
	//GetConsoleWindowAPI = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");


	//登录参数，包括设备地址、登录用户、密码等
//	NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy_s(struLoginInfo.sDeviceAddress, "192.168.0.252"); //设备IP地址
	struLoginInfo.wPort = 8000; //设备服务端口
	strcpy_s(struLoginInfo.sUserName, "admin"); //设备登录用户名
	strcpy_s(struLoginInfo.sPassword, "Changeme"); //设备登录密码

												 //设备信息, 输出参数
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
	//获取压缩参数
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
		cout << "获取设备基本配置参数失败!" << NET_DVR_GetLastError ()<< endl;
	}
	cout<<m_struDeviceCfg.byDiskNum<<endl;//硬盘数
	//m_iAlarmInNum = m_struDeviceCfg.byAlarmInPortNum;//报警输入数
	//m_iAlarmOutNum = m_struDeviceCfg.byAlarmOutPortNum;//报警输出数
	//m_iChanNum = m_struDeviceCfg.byChanNum;//通道数
	//m_iYKQID = m_struDeviceCfg.dwDVRID;//遥控器ID

	return false;
}

bool HIK::EndPreview()
{
	//关闭预览
	NET_DVR_StopRealPlay(lRealPlayHandle);

	return false;
}

bool HIK::StartPreview()
{
	//启动预览并设置回调数据流
	//LONG lRealPlayHandle;
	//HWND hWnd = GetConsoleWindowAPI();     //获取窗口句柄
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	//struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1;       //预览通道号
	struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 1;       //0- 非阻塞取流，1- 阻塞取流

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
