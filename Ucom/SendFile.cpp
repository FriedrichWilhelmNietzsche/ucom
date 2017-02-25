// SnedFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "SendFile.h"
#include "afxdialogex.h"


// CSnedFile �Ի���

IMPLEMENT_DYNAMIC(CSendFile, CDialog)

CSendFile::CSendFile(CWnd* pParent, HANDLE *hUART)
: CDialog(CSendFile::IDD, pParent)
{
	hUartCom = hUART;
}

CSendFile::~CSendFile()
{
}

void CSendFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CSendFile::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("�ļ�����"));
	strFilePath = "";
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PrgSend);
	pProgress->SetStep(1);
	pProgress->SetRange(0, 10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CSendFile, CDialog)
	ON_BN_CLICKED(IDC_BtnLoad, &CSendFile::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BtnSendFile, &CSendFile::OnBnClickedBtnsendfile)
END_MESSAGE_MAP()


// CSendFile ��Ϣ�������
void CSendFile::AppendLog(CString str)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EdbFileLog);
	pEdit->SetSel(-1, -1);
	pEdit->ReplaceSel(str);
}

void CSendFile::OnBnClickedBtnload()
{
	CFileDialog fDlg(
		true, // FALSEΪ�洢�ļ�
		NULL, // Ĭ����չ��
		NULL, // Ĭ���ļ���
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // ����ֻ����ѡ�Ϳ����ɸ�д����
		_T("�ı��ļ�(*.txt)|*.txt|All Files (*.*)|*.*||"), // ��׺����
		NULL);

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PrgSend);
	pProgress->SetPos(0);

	if (IDOK == fDlg.DoModal())
	{
		strFilePath = fDlg.GetPathName();
		CString strtmp;
		strtmp += ">> "+fDlg.GetFileName()+" ����ɹ�\r\n";
		AppendLog(strtmp);
	}
}

#define MAX_PIC_Bytes 1024
void CSendFile::OnBnClickedBtnsendfile()
{
	CString strtmp;
	if (strFilePath.IsEmpty())
	{
		strtmp += ">> Error���ļ�������\r\n";
		AppendLog(strtmp);
	}
	else
	{
		CFile mFile;
		if (mFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
		{
			ULONGLONG lBytes=mFile.GetLength();
			UINT nBytes = (UINT)lBytes;
			UINT nCnt = nBytes / MAX_PIC_Bytes;
			UINT rBytes = nBytes % MAX_PIC_Bytes;
			CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PrgSend);

			char buf[MAX_PIC_Bytes + 1];
			if (nBytes > MAXUINT)
			{
				strtmp+=">> Error���ļ�����\r\n";
				AppendLog(strtmp);
				return;
			}
			strtmp.Format(">> �ļ���С��%d B\r\n<< ���ڷ���...\r\n",nBytes);
			AppendLog(strtmp);

			if (nBytes < MAX_PIC_Bytes)
			{
				pProgress->SetRange(0, 1);
				pProgress->SetPos(0);

				mFile.Read(buf, nBytes);
				UnblockSend(buf, nBytes);
				
				pProgress->SetPos(1);
			}
			else
			{
				pProgress->SetRange(0, nCnt);
				pProgress->SetPos(0);
				TRACE("loop:%d\n", nCnt);

				for (UINT i = 0; i < nCnt; i++)
				{
					mFile.Read(buf, MAX_PIC_Bytes);
					pProgress->SetPos(i);
					UnblockSend(buf, MAX_PIC_Bytes);
				}
				if (rBytes != 0)
				{
					mFile.Read(buf, rBytes);
					UnblockSend(buf, rBytes);
					pProgress->SetPos(nCnt);
				}
			}
			strtmp=">> ���ͳɹ���\r\n--------------------���Ƿָ���--------------------\r\n";
			AppendLog(strtmp);
			mFile.Close();
		}
	}
}

int CSendFile::UnblockSend(const char*pBuff, UINT len)
{
	BOOL bWriteStatus;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwLength;

	ClearCommError(*hUartCom, &dwErrorFlags, &ComStat);
	if (dwErrorFlags>0)
	{
		TRACE("Unblock Write Failed\n");
		PurgeComm(*hUartCom, PURGE_TXABORT | PURGE_TXCLEAR);
		return 0;
	}
	m_osWrite.Offset = 0;

	bWriteStatus = WriteFile(*hUartCom, pBuff, len, &dwLength, &m_osWrite);

	if (!bWriteStatus)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//����ص�����δ���,�ȴ�ֱ���������
			GetOverlappedResult(*hUartCom, &m_osWrite, &dwLength, TRUE);
		}
		else
			dwLength = 0;
	}
	return dwLength;
}


void CSendFile::OnOK()
{
}
