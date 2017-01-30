// choice1.cpp : Defines the entry point for the DLL application.
// ���ʵ��

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

//��ȡ�ص�����
void RegisterDataInterface(PDATAIOFUNC pfn)
{
	g_pFuncCallBack = pfn;
}

//ע������Ϣ
void GetCopyRightInfo(LPPLUGIN info)
{
	//��д������Ϣ
	strcpy(info->Name,"β������");
	strcpy(info->Author,"����ƽ");
	strcpy(info->Period,"����");
	strcpy(info->Descript,"β������");
	//��д������Ϣ
	info->ParamNum = 4;
	strcpy(info->ParamInfo[0].acParaName,"��������");
	info->ParamInfo[0].nMax=29999999;
	info->ParamInfo[0].nMin=20100101;
	info->ParamInfo[0].nDefault=20160101;
	strcpy(info->ParamInfo[1].acParaName,"������ʱ��");
	info->ParamInfo[1].nMax=10000;
	info->ParamInfo[1].nMin=1;
	info->ParamInfo[1].nDefault=1;
	strcpy(info->ParamInfo[2].acParaName,"��������");
	info->ParamInfo[2].nMax=100;
	info->ParamInfo[2].nMin=1;
	info->ParamInfo[2].nDefault=1;
	strcpy(info->ParamInfo[3].acParaName,"���������Ϣ");
	info->ParamInfo[3].nMax=1;
	info->ParamInfo[3].nMin=0;
	info->ParamInfo[3].nDefault=0;
}

BOOL InputInfoThenCalc1(char * Code,short nSetCode,int Value[4],short DataType,short nDataNum,BYTE nTQ,unsigned long unused) //��������ݼ���
{
	int o = Value[3];
	tdxLog("/*******************start****************************/",o);
	tdxLog("��ǰ��Ʊ " + charPointToString(Code),o);
	BOOL nRet = FALSE;
	
	string expectedTime = intToString(Value[0]);
	tdxLog("����β���������� " + expectedTime, o);
	string expectedYear = expectedTime.substr(0, 4);
	string expectedMonth = expectedTime.substr(4, 2);
	string expectedDay = expectedTime.substr(6, 2);

	int y = atoi(expectedYear.c_str());
	int m = atoi(expectedMonth.c_str());
	int d = atoi(expectedDay.c_str());

	NTime tmpTime={y,m,d};
	//tdxLog(expectedYear + " " + expectedMonth + " " + expectedDay);
	int T = Value[1];
	tdxLog("��������ʱ�� " + intToString(T),o);
	int B = Value[2];
	tdxLog("���� " + intToString(B), o);

	LPHISDAT pHisDat = new HISDAT[nDataNum];  

	long readnum = g_pFuncCallBack(Code,nSetCode,DataType,pHisDat,nDataNum,tmpTime,tmpTime,nTQ,0);  //���ûص������������ݣ����صõ������ݸ���

	tdxLog("���ж��������� " + longToString(readnum),o);

	int counter = 0;
	float endClose = 0;
	for (int i = readnum - 1;i >= 0; i--) {
		
		if (counter == 0) {
			//15:00�ļ۸�
			endClose = pHisDat[i].Close;
			tdxLog("���̼� " + floatToString(endClose),o);
		}
		
		if (counter == T) {
			tdxLog("�����ľ�����ʱ��" + unsignedShortToString(pHisDat[i].Time.hour) + " " + unsignedShortToString(pHisDat[i].Time.minute),o);
			tdxLog("��ʱ�۸� " + floatToString(pHisDat[i].Close),o);
			tdxLog("�������� " + floatToString((endClose - pHisDat[i].Close) / pHisDat[i].Close * 1000),o);
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

BOOL InputInfoThenCalc2(char * Code,short nSetCode,int Value[4],short DataType,NTime time1,NTime time2,BYTE nTQ,unsigned long unused)  //ѡȡ����
{
	return false;
}
