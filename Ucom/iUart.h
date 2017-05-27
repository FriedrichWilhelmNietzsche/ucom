#pragma once

#define WM_COMM_RX_MSG  (WM_USER + 2)

typedef struct
{
	//���ھ��
	HANDLE *commHandle;
	//�߳���ֹ��־
	int stopFlag;
}ThreadPara;

///�߳�����
//�����������˳�
#define RT_REQ_EXIT 0
//�����������˳�
#define RT_PRE_EXIT 1
//�������������˳�
#define RT_NOT_EXIT 2
//�������˳��ɹ�
#define RT_SUC_EXIT 3


class iUart
{
private:
	HANDLE hUartCom;
	DCB uartConfig;
	CString ComName;

	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	ThreadPara mThreadPara;
	void CleanCommErr(void);
public:

	iUart(){
		hUartCom = NULL;
		mThreadPara.commHandle = &hUartCom;
	}
	~iUart();

	void ClosePort(void){
		CloseHandle(hUartCom);
		hUartCom = NULL;
		TRACE("Close PortHandle\n");
	}

	bool OpenCom(bool isBlockMode = true);
	bool ConfigUart(CString comName, DCB mConfig);

	void GetComList(CComboBox *cblist);

	int WriteCString(const CString &cBuffer);
	CString ReadCString(void);

	int UnblockRead(CString &dataStr);
	int UnblockSend(const CString &dataStr);

	HANDLE GetHandle(void){
		return hUartCom;
	}
	HANDLE* GetHandleAddr(void){
		return &hUartCom;
	}

	bool isConnected(void){
		if (hUartCom == NULL)
			return false;
		else
			return true;
	}
	ThreadPara *GetThreadStartPara(void){
		mThreadPara.stopFlag = RT_PRE_EXIT;
		return &mThreadPara;
	}

};


UINT RxThreadFunc(LPVOID mThreadPara);

