#pragma once


// CSnedFile �Ի���

class CSendFile : public CDialog
{
	DECLARE_DYNAMIC(CSendFile)

public:
	CSendFile(CWnd* pParent, HANDLE *hUART);   // ��׼���캯��
	virtual ~CSendFile();

// �Ի�������
	enum { IDD = IDD_FILE_LOAD };

private:
	CString strFilePath;

	HANDLE *hUartCom;
	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	int UnblockSend(const char*pBuff, UINT len);
	void CSendFile::AppendLog(CString str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnload();
	afx_msg void OnBnClickedBtnsendfile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
