// choice1.cpp : Defines the entry point for the DLL application.
// 插件实例

#include "stdafx.h"
#include "plugin.h"

#define PLUGIN_EXPORTS

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

PDATAIOFUNC	 g_pFuncCallBack;

//获取回调函数
void RegisterDataInterface(PDATAIOFUNC pfn)
{
	g_pFuncCallBack = pfn;
}

//注册插件信息
void GetCopyRightInfo(LPPLUGIN info)
{
	//填写基本信息
	strcpy(info->Name,"尾盘拉升");
	strcpy(info->Author,"李秋平");
	strcpy(info->Period,"短线");
	strcpy(info->Descript,"尾盘拉升");
	//填写参数信息
	info->ParamNum = 4;
	strcpy(info->ParamInfo[0].acParaName,"期望日期");
	info->ParamInfo[0].nMax=29999999;
	info->ParamInfo[0].nMin=20100101;
	info->ParamInfo[0].nDefault=20160101;
	strcpy(info->ParamInfo[1].acParaName,"距收盘时间");
	info->ParamInfo[1].nMax=10000;
	info->ParamInfo[1].nMin=1;
	info->ParamInfo[1].nDefault=1;
	strcpy(info->ParamInfo[2].acParaName,"拉升倍数");
	info->ParamInfo[2].nMax=100;
	info->ParamInfo[2].nMin=1;
	info->ParamInfo[2].nDefault=1;
	strcpy(info->ParamInfo[3].acParaName,"输出调试信息");
	info->ParamInfo[3].nMax=1;
	info->ParamInfo[3].nMin=0;
	info->ParamInfo[3].nDefault=0;
}

BOOL InputInfoThenCalc1(char * Code,short nSetCode,int Value[4],short DataType,short nDataNum,BYTE nTQ,unsigned long unused) //按最近数据计算
{
	int o = Value[3];
	tdxLog("/*******************start****************************/",o);
	tdxLog("当前股票 " + charPointToString(Code),o);
	BOOL nRet = FALSE;
	
	string expectedTime = intToString(Value[0]);
	tdxLog("期望尾盘拉升日期 " + expectedTime, o);
	string expectedYear = expectedTime.substr(0, 4);
	string expectedMonth = expectedTime.substr(4, 2);
	string expectedDay = expectedTime.substr(6, 2);

	int y = atoi(expectedYear.c_str());
	int m = atoi(expectedMonth.c_str());
	int d = atoi(expectedDay.c_str());

	NTime tmpTime={y,m,d};
	//tdxLog(expectedYear + " " + expectedMonth + " " + expectedDay);
	int T = Value[1];
	tdxLog("距离收盘时间 " + intToString(T),o);
	int B = Value[2];
	tdxLog("倍数 " + intToString(B), o);

	LPHISDAT pHisDat = new HISDAT[nDataNum];  

	long readnum = g_pFuncCallBack(Code,nSetCode,DataType,pHisDat,nDataNum,tmpTime,tmpTime,nTQ,0);  //利用回调函数申请数据，返回得到的数据个数

	tdxLog("共有多少条数据 " + longToString(readnum),o);

	int counter = 0;
	float endClose = 0;
	for (int i = readnum - 1;i >= 0; i--) {
		
		if (counter == 0) {
			//15:00的价格
			endClose = pHisDat[i].Close;
			tdxLog("收盘价 " + floatToString(endClose),o);
		}
		
		if (counter == T) {
			tdxLog("期望的距收盘时间" + unsignedShortToString(pHisDat[i].Time.hour) + " " + unsignedShortToString(pHisDat[i].Time.minute),o);
			tdxLog("此时价格 " + floatToString(pHisDat[i].Close),o);
			tdxLog("拉升比例 " + floatToString((endClose - pHisDat[i].Close) / pHisDat[i].Close * 1000),o);
			if ((endClose - pHisDat[i].Close) / pHisDat[i].Close * 1000 > B) {
				nRet =TRUE;
			}
		}
		counter++;
	}
	delete []pHisDat;pHisDat=NULL;
	tdxLog("/*******************end****************************/",o);
	return nRet;
}

BOOL InputInfoThenCalc2(char * Code,short nSetCode,int Value[4],short DataType,NTime time1,NTime time2,BYTE nTQ,unsigned long unused)  //选取区段
{
	return false;
}
