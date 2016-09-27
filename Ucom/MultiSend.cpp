// MultiSend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "MultiSend.h"
#include "afxdialogex.h"


// CMultiSend �Ի���

IMPLEMENT_DYNAMIC(CMultiSend, CDialog)

CMultiSend::CMultiSend(CWnd* pParent, bool *pUartOpenStatus, HANDLE *hUART)
	: CDialog(CMultiSend::IDD, pParent)
{
	pIsUartOpen = pUartOpenStatus;
	hUartCom = hUART;
}

CMultiSend::~CMultiSend()
{
}

void CMultiSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMultiSend, CDialog)
	ON_BN_CLICKED(IDC_CkbTimeXSend, &CMultiSend::OnBnClickedCkbtimexsend)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EdbTimeXSend, &CMultiSend::OnEnChangeEdbtimexsend)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BtnSend1, IDC_BtnSend10, OnBnClickedBtnsend)
	ON_CBN_DROPDOWN(IDC_CbTransNum, &CMultiSend::OnDropdownCbtransnum)
	ON_BN_CLICKED(IDC_BtnTrans, &CMultiSend::OnBnClickedBtntrans)
END_MESSAGE_MAP()


BOOL CMultiSend::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox *pCombox;

	//���÷Ƕ�������
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	SetDlgItemInt(IDC_EdbTimeXSend, 1000);

	//�趨���ڲ���
	pCombox = (CComboBox*)GetDlgItem(IDC_CbTransNum);
	char str[2] = "0";
	for (int i = 0; i < 10; i++)
	{
		str[0] = '0' + i;
		pCombox->AddString(str);
	}
	pCombox->SetCurSel(0);
	return TRUE;
	// �쳣:  OCX ����ҳӦ���� FALSE
}


// CMultiSend ��Ϣ�������
void CMultiSend::LoppSendSet(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbTimeXSend))
	{
		int time = GetDlgItemInt(IDC_EdbTimeXSend);
		//����ѡ��
		if (time <= 0 || *pIsUartOpen == FALSE)
		{
			((CButton *)GetDlgItem(IDC_CkbTimeXSend))->SetCheck(BST_UNCHECKED);
				return;
		}
		TRACE("Set Time\n");
		SetTimer(LOOP_SEND_ID, time, NULL);
	}
	else
	{
		KillTimer(LOOP_SEND_ID);
		OnTimeSend(true);
	}

}


void CMultiSend::OnEnChangeEdbtimexsend()
{
	LoppSendSet();
}
void CMultiSend::OnBnClickedCkbtimexsend()
{
	LoppSendSet();
}

int CMultiSend::UnblockSend(const CString &dataStr)
{
	BOOL bWriteStatus;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwLength;
	if (*pIsUartOpen == FALSE)
		return -1;

	ClearCommError(*hUartCom, &dwErrorFlags, &ComStat);
	if (dwErrorFlags>0)
	{
		TRACE("Unblock Write Failed\n");
		PurgeComm(*hUartCom, PURGE_TXABORT | PURGE_TXCLEAR);
		return 0;
	}
	m_osWrite.Offset = 0;
	dwLength = dataStr.GetAllocLength();
	bWriteStatus = WriteFile(*hUartCom, dataStr, dwLength, &dwLength, &m_osWrite);

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

void CMultiSend::OnTimeSend(bool clearcnt)
{
	static int cnt=0,lastCnt=9;

	if (clearcnt)
	{
		GetDlgItem(IDC_EdbData1 + lastCnt)->EnableWindow(true);
		cnt = 0, lastCnt = 9;
		return;
	}
	else
	{
		GetDlgItem(IDC_EdbData1 + lastCnt)->EnableWindow(true);
		GetDlgItem(IDC_EdbData1 + cnt)->EnableWindow(false);

		//��������
		SendEdbox(cnt);

		lastCnt = cnt;
		if (++cnt == 10)
			cnt = 0;
	}
}

void CMultiSend::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case LOOP_SEND_ID:
		OnTimeSend();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMultiSend::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
	return;
}

//��0��ʼ
void CMultiSend::SendEdbox(int index)
{
	bool isHex=false;
	CString strTmp;
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbXHex1+index))
		isHex = true;

	GetDlgItemText(IDC_EdbData1 + index, strTmp);
	txData.ReString(strTmp, isHex);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbXNewLine))
	{
		strTmp = "\r\n";
		txData.AppendString(strTmp);
	}
	UnblockSend(txData.GetCStrData());
}

void CMultiSend::OnBnClickedBtnsend(UINT   uId)
{
	int btnNum = uId - IDC_BtnSend1;
	SendEdbox(btnNum);
}


void CMultiSend::OnDropdownCbtransnum()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbTransNum);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}


void CMultiSend::OnBnClickedBtntrans()
{
	CString strTmp;

	GetDlgItemText(IDC_EdbXDetail,strTmp); 
	SetDlgItemText(IDC_EdbData1 + ((CComboBox *)GetDlgItem(IDC_CbTransNum))->GetCurSel()
		, strTmp);
}
