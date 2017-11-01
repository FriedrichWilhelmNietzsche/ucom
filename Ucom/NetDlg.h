#pragma once


#include "UBase.h"
#include "iSocket.h"

// CNetDlg �Ի���

class CNetDlg : public CDialog, public UcomBase
{
	DECLARE_DYNAMIC(CNetDlg)

private:
	//�����߳̾��
	HANDLE hRxThread;
	//bool IsSocketAvailable(void);
	static iSocket mSocket;
	static bool netSocketIsOpen;
	//static iUart mUart;

public:
	CNetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetDlg();

	void InitPanel(void);
	void SwPanel(bool choose);
	void EnableIPIn(bool isDisable);
	void ChangeBmpPic(int PicCtrlID, unsigned short nPicID);
	void SetTips(CString tips) {
		SetDlgItemText(IDC_TxtIPInfo, tips);
	}
	void GetDstIPStr(CString &str);

	void LoadRegConfig();
	// �����ڹر�ʱ��OnClose��Ҫ���ñ���һ����Ϣ
	void WriteRegData(void);

	//client�б���
	void DelClient(nSocketPara *nPara);
	void AddClient(nSocketPara *nPara);
	void CleanClient(void);

	void OpenSocket(void);

	//static int AsyncSend(const CString &dataStr) {
	//	return mSocket.UnblockSend(dataStr);
	//}
	//static int AsyncRead(CString &dataStr) {
	//	return mSocket.UnblockRead(dataStr);
	//}
	//int VirtualSend(CString &dataStr);
	//static bool isOpen(void) {
	//	return mSocket.IsScoketOpen();
	//}
	int AsyncSend(const CString &dataStr) {
		return mSocket.UnblockSend(dataStr);
	}
	int AsyncRead(CString &dataStr) {
		return mSocket.UnblockRead(dataStr);
}
	int VirtualSend(CString &dataStr);
	bool isOpen(void) {
		return mSocket.IsScoketOpen();
	}

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnConnect();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditPortComplete();
	afx_msg LRESULT CNetDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelNetType();
	afx_msg void OnSelChangeCient();
};
