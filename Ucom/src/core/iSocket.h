#pragma once

#include <afxsock.h>
#include "UBase.h"
#include "DeConsole.h"

#define ISOCKET_READBUFF_MAX 4086

typedef struct
{
	CString strIP;
	void *hSocket;
} nSocketPara;


// client�ͻ����࣬����ISocket��serverģʽ�Ķ�̬����
class nSocket :public CAsyncSocket
{
private:
	HWND hwParent;
	HWND hwTopParent;
	nSocketPara mPara;
public:
	nSocket(HWND pParent, HWND pTopParent) {
		hwParent = pParent;
		hwTopParent = pTopParent;
	}
	~nSocket();

	void SetSocketHandle(void *hSock) {
		mPara.hSocket = hSock;
	}
	void OnReceive(int nErrorCode);

	void OnSend(int nErrorCode);

	void OnConnect(int nErrorCode);

	void OnClose(int nErrorCode);

	void PostMsgIsOpen(bool isOpen);
	void GetClientInfo(void);
	int UnblockRead(CString &dataStr);
};

// ���ӵ�����server��client��
class iSocket :public CAsyncSocket
{
private:
	//�Ƿ���Զ�д����
	bool isSocketOpen;
	//�����ھ��������������ڷ�����Ϣ
	HWND hwParent;
	HWND hwTopParent;
public:
	iSocket();
	~iSocket();
	CArray<nSocket *> nClient;
	void SetParentHWND(HWND pParent, HWND pTopParent) {
		hwParent = pParent;
		hwTopParent = pTopParent;
	}
	void DelFromClient(nSocket *mclient);
	void DelAllClient(void);

	void OnReceive(int nErrorCode);

	void OnAccept(int nErrorCode);

	void OnConnect(int nErrorCode);

	void OnClose(int nErrorCode);

	void PostMsgIsOpen(bool isOpen);
	int UnblockRead(CString &dataStr);
	int UnblockRead(CString & dataStr, CString & infoStr);
	int UnblockSend(const CString & dataStr)
	{
		return Send((LPCTSTR)dataStr, dataStr.GetLength());
	}

	bool IsScoketOpen(void) {
		return isSocketOpen;
	}
	// ��Ҫ����UDP����£���Ҫ����ֶ��򿪹ر�
	void SetIsOpen(bool isopen) {
		isSocketOpen = isopen;
	}
};