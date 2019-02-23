#pragma once

#include "UBase.h"

// CSnedFile �Ի���
class CSendFile : public CDialog
{
	DECLARE_DYNAMIC(CSendFile)

public:
	CSendFile::CSendFile(CWnd* pParent, UcomBase** mbase);
	virtual ~CSendFile();

// �Ի�������
	enum { IDD = IDD_FILE_LOAD };

private:
	CString strFilePath;
	int UnblockSend(const char*pBuff, UINT len);
	void AppendLog(CString str);

	// ģ̬�Ի���ֻ���ڴ�ͨ�Ŷ˿�����µ������Բ���Ҫ�ж��Ƿ�򿪶˿�
	UcomBase** uuBase;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnload();
	afx_msg void OnBnClickedBtnsendfile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
