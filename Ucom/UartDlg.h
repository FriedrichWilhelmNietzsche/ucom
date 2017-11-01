#pragma once

#include "iUart.h"


// CUartDlg �Ի���
class CUartDlg : public CDialog
{
	DECLARE_DYNAMIC(CUartDlg)

private:

	//�����߳̾��
	HANDLE hRxThread;
	bool IsUartPortAvailable(void);
	static bool uartPortIsOpen;
	static iUart mUart;

public:

	void InitPanel(void);
	void SwPanel(bool choose);
	void ChangeBmpPic(int PicCtrlID, unsigned short nPicID);

	void OpenUart();

	void OnDropdownCbUartPort();
	void OnSelendokCbUartPort();

	DCB GetUartConfigDCB(void);
	void LoadRegConfig();
	void GetRegData(CString &comName, CString &dcbConfig);
	// �����ڹر�ʱ��OnClose��Ҫ���ñ���һ����Ϣ
	void WriteRegData(void);


	static int AsyncSend(const CString &dataStr) {
		return mUart.UnblockSend(dataStr);
	}
	static int AsyncRead(CString &dataStr) {
		return mUart.UnblockRead(dataStr);
	}
	static bool isOpen(void) {
		return uartPortIsOpen;
	}
public:
	CUartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUartDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnOpen();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPicuartstatus();
};
