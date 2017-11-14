#pragma once
#include"head.h"
#include"GeneralDef.h"""
//#include"hik\HCNetSDK.h"
#include"time.h"
using namespace std;


//typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();
//PROCGETCONSOLEWINDOW GetConsoleWindowAPI;

//HCNetSDK.lib
//PlayCtrl.lib


//#pragma  comment(lib, "HCNetSDK.lib")
//#pragma  comment(lib, "PlayCtrl.lib")

//void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
//{
//	char tempbuf[256] = { 0 };
//	switch (dwType)
//	{
//	case EXCEPTION_RECONNECT:    //预览时重连
//		printf("----------reconnect--------%d\n", time(NULL));
//		break;
//	default:
//		break;
//	}
//}


class HIK
{
public:
	HIK();
	~HIK();
	void Init();
	bool GetCompressionParameters();
	bool GetBasicParameters();
	bool StartPreview();
	bool EndPreview();
private:
	LONG lUserID;
	NET_DVR_USER_LOGIN_INFO struLoginInfo;
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40;//网络参数配置结构体
	NET_DVR_DEVICECFG m_struDeviceCfg;//设备参数配置结构体
	//NET_DVR_GET_TIMECFG test;
	//NET_DVR_GET_DEVICECFG_V40 t;
	LONG lRealPlayHandle;
	LOCAL_DEVICE_INFO struDeviceInfo;
};

