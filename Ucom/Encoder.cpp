// Encoder.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "Encoder.h"
#include "afxdialogex.h"


// CEncoder �Ի���

IMPLEMENT_DYNAMIC(CEncoder, CDialog)

CEncoder::CEncoder(CWnd* pParent /*=NULL*/)
	: CDialog(CEncoder::IDD, pParent)
	, m_RadioEnc(0)
{

}

CEncoder::~CEncoder()
{
}

void CEncoder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_GrpEnc, mEncDisp);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioEnc);
}


BEGIN_MESSAGE_MAP(CEncoder, CDialog)
	ON_EN_CHANGE(IDC_EncInput, &CEncoder::OnEnChangeEncinput)
	ON_BN_CLICKED(IDC_EncClear, &CEncoder::OnBnClickedEncclear)
	ON_BN_CLICKED(IDC_RADIO1, &CEncoder::OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CEncoder::OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CEncoder::OnClickedRadio1)
END_MESSAGE_MAP()


// CEncoder ��Ϣ�������

const char CEncoder::HexTable[] = "0123456789ABCDEF";

CString CEncoder::Ascii2Unicode(CString &strascii)
{
	CString res, dataStr("Unicode����(16λС��):\r\n");
	int nBytes = strascii.GetLength() * 2 + 2;
	res.GetBufferSetLength(nBytes);
	MultiByteToWideChar(CP_ACP, 0, (LPCCH)strascii.GetBuffer(0), -1, (LPWSTR)res.GetBuffer(0), nBytes);

	const unsigned char *pCh = (const unsigned char *)(res.GetBuffer(0));
	//ȥ��ֹͣ��0x0000
	//nBytes = res.GetLength() - 2;

	char HexCh[] = "0xHLHL,";
	if (m_RadioEnc == 2)
		HexCh[6] = ' ';

	while (nBytes)
	{
		HexCh[2] = HexTable[*(pCh + 1) / 16];
		HexCh[3] = HexTable[*(pCh + 1) % 16];

		HexCh[4] = HexTable[(*pCh) / 16];
		HexCh[5] = HexTable[(*pCh) % 16];
		if (m_RadioEnc == 0)
			dataStr += (HexCh);
		else
			dataStr += (HexCh + 2);

		pCh += 2;
		nBytes -= 2;
		if (*(pCh + 1) == '\0'&&*pCh == '\0')
			break;
	}
	dataStr.Delete(dataStr.GetLength() - 1, 1);
	dataStr += "\r\n\r\n";
	return dataStr;
}


CString CEncoder::Ascii2Utf8(CString &strascii)
{
	CString res, dataStr;
	int nBytes = strascii.GetLength() * 2 + 2;
	//��תΪUnicode
	res.GetBufferSetLength(nBytes);
	MultiByteToWideChar(CP_ACP, 0, strascii.GetBuffer(0), -1, (LPWSTR)res.GetBuffer(0), nBytes);

	nBytes = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)res.GetBuffer(0), -1, NULL, 0, NULL, NULL);
	dataStr.GetBufferSetLength(nBytes);
	WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)res.GetBuffer(0), -1, dataStr.GetBuffer(0), nBytes, NULL, NULL);

	const unsigned char *pCh = (const unsigned char *)(dataStr.GetBuffer(0));
	//ȥ��ֹͣ��0x00
	nBytes--;
	res.Empty();
	res = "Utf8����:\r\n";

	char HexCh[] = "0xHL,";
	if (m_RadioEnc == 2)
		HexCh[4] = ' ';
	
	while (nBytes--)
	{
		HexCh[2] = HexTable[(*pCh) / 16];
		HexCh[3] = HexTable[(*pCh) % 16];
		if (m_RadioEnc == 0)
			res += HexCh;
		else
			res += (HexCh+2);

		pCh++;
	}
	res.Delete(res.GetLength() - 1, 1);

	res += "\r\n\r\n";
	return res;
}

CString CEncoder::GetAsciiCode(CString &str)
{
	CString DataStr("ASCII����:\r\n");
	const unsigned char *pCh = (const unsigned char *)(str.GetBuffer(0));
	int nBytes = str.GetAllocLength();

	char HexCh[] = "0xHL,";
	if (m_RadioEnc == 2)
		HexCh[4] = ' ';
	while (nBytes--)
	{
		HexCh[2] = HexTable[(*pCh) / 16];
		HexCh[3] = HexTable[(*pCh) % 16];
		if (m_RadioEnc == 0)
			DataStr += HexCh;
		else
			DataStr += (HexCh + 2);
		pCh++;
	}
	DataStr.Delete(DataStr.GetLength() - 1, 1);
	DataStr += "\r\n\r\n";
	return DataStr;
}
CString CEncoder::Unicode2Ascii(const CString &dataStr)
{
	int nBytes;
	CString res, uniStr = dataStr;
	uniStr = uniStr + '\0'+'\0';

	nBytes = -1 + WideCharToMultiByte(CP_ACP, 0, (LPWSTR)uniStr.GetBuffer(0), -1, NULL, 0, NULL, 0);
	res.GetBufferSetLength(nBytes);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)uniStr.GetBuffer(0), -1, res.GetBuffer(0), nBytes, NULL, 0);
	nBytes = res.GetAllocLength();
	for (int i=0; i < nBytes;i++)
	{
		if (res[i] == '\0')
			res.Delete(i, 1);//ɾ��UNICODE����ascii���ɵ�'\0'
	}

	return res;
}


CString CEncoder::Utf8toAscii(const CString &dataStr)
{
	int nBytes;
	CString res, uniStr = dataStr;
	uniStr = uniStr + '\0';
	//��תΪUnicode
	nBytes = MultiByteToWideChar(CP_UTF8, 0, uniStr.GetBuffer(0), -1, NULL, 0);
	res.GetBufferSetLength(2*nBytes);

	MultiByteToWideChar(CP_UTF8, 0, uniStr.GetBuffer(0), -1, (LPWSTR)res.GetBuffer(0), nBytes);
	uniStr = Unicode2Ascii(res);
	return uniStr;
}



void CEncoder::OnEnChangeEncinput()
{
	CString strtmp;
	GetDlgItemText(IDC_EncInput, strtmp);
	SetDlgItemText(IDC_EncOuput, GetAsciiCode(strtmp) + Ascii2Utf8(strtmp) + Ascii2Unicode(strtmp));
}


void CEncoder::OnBnClickedEncclear()
{
	SetDlgItemText(IDC_EncInput, "");
	SetDlgItemText(IDC_EncOuput, "");
}


void CEncoder::OnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	OnEnChangeEncinput();
}


void CEncoder::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
	return;

}
