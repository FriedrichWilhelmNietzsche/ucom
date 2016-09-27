// GraphDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


// CGraphDlg �Ի���

IMPLEMENT_DYNAMIC(CGraphDlg, CDialog)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGraphDlg::IDD, pParent), isGraph(false)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	ON_BN_CLICKED(IDC_BtnClearGraph, &CGraphDlg::OnBnClickedBtncleargraph)
	ON_WM_PAINT()
//	ON_STN_CLICKED(IDC_PIC_DZXH, &CGraphDlg::OnStnClickedPicDzxh)
ON_BN_CLICKED(IDC_BtnStartGraph, &CGraphDlg::OnBnClickedBtnstartgraph)
END_MESSAGE_MAP()


//void CGraphDlg::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO:  �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
//}


void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	hCanvas = GetDlgItem(IDC_CANVAS)->m_hWnd;
	GetDlgItem(IDC_CANVAS)->GetClientRect(&rectCanvas);
	pCanvas = (GetDlgItem(IDC_CANVAS)->GetDC());
	//hdc = ::BeginPaint(hCanvas, &ps);
	//::EndPaint(hCanvas, &ps);
}


BOOL CGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCChart();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CGraphDlg::InitCChart(void)
{

}

void CGraphDlg::AddDataString(CString str)
{
	if (!isGraph)
		return;
	int lastSpacePos, nowSpacePos;
	int index;
	double data;
	CString tmp;
	static int cnt;

	nowSpacePos = 0;
	for (int i = 0; i < 4; i++)
	{
		lastSpacePos = str.Find(' ', nowSpacePos) + 1;
		
		if (lastSpacePos == 0)
			return;	//û�пո�֡����

		nowSpacePos = str.Find(' ', lastSpacePos);
		if (nowSpacePos == -1)
		{	//ĩβ֡
			nowSpacePos = str.GetLength();
		}

		//�ַ������ȵô���3������֡����
		if (nowSpacePos - lastSpacePos < 3)
			return;

		tmp = str.Mid(lastSpacePos, nowSpacePos - lastSpacePos) + '\0';
		//����ȡ������Ч����ʱ��ȷ
		if (sscanf_s(tmp.GetBuffer(0), "%d:%lf", &index, &data) == 2)
		{
			SetDlgItemText(IDC_EdbData0+i, tmp);
			TRACE("index:%d data:%lf\r\n", index, data);
			
			
		}
		else
			return;	//֡����
		//����һ֡
		nowSpacePos=lastSpacePos;
	}
	cnt++;

	InvalidateRect(&rectCanvas, true);
}


void CGraphDlg::OnBnClickedBtncleargraph()
{

	InvalidateRect(&rectCanvas, true);
}


void CGraphDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
	return;

}


void CGraphDlg::OnBnClickedBtnstartgraph()
{
	isGraph = !isGraph;
	if (isGraph)
		SetDlgItemText(IDC_BtnStartGraph, "ֹͣ");
	else
		SetDlgItemText(IDC_BtnStartGraph, "��ʼ");
}
