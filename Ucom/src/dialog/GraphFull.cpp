// GraphFull.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "GraphFull.h"
#include "afxdialogex.h"


// CGraphFull �Ի���

IMPLEMENT_DYNAMIC(CGraphFull, CDialog)

CGraphFull::CGraphFull(CChartCtrl *pChart, CWnd* pParent)
	: CDialog(IDD_GraphFull, pParent)
{
	pChartCtrl = pChart;
}

CGraphFull::~CGraphFull()
{
}

void CGraphFull::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ChartFull, *pChartCtrl);
}


BEGIN_MESSAGE_MAP(CGraphFull, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGraphFull ��Ϣ�������


void CGraphFull::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_ChartFull);
	if (pWnd != NULL)
	{
		CRect rec;
		GetClientRect(&rec);
		pWnd->MoveWindow(rec);
	}
}


void CGraphFull::DoDDX_Control(bool isAttach)
{
	if (isAttach) {
		//�ػ�
		pChartCtrl->SubclassDlgItem(IDC_ChartFull, this);
		OnSize(SIZE_RESTORED, 0, 0);
	}
	else {
		if (pChartCtrl->GetSafeHwnd())
			pChartCtrl->UnsubclassWindow();
	}
}

BOOL CGraphFull::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CMenu* pMenu = this->GetSystemMenu(FALSE);//ϵͳ�˵�
	//pMenu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);//���ùرհ�ť

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CGraphFull::OnClose()
{
	//CDialog::OnClose();

	//������Ϣ�˳������ø����ڹرձ�����
	::SendMessage(pMianHwnd, WM_COMM_CLOSE_ME_MSG, 0, 0);
}
//���ߴ绹ԭ����Ƕ�Ĵ�С��������ʾ��ȫ
void CGraphFull::Resize(LONG Bottom, LONG Right)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_ChartFull);
	if (pWnd != NULL)
	{
		CRect rec;
		GetClientRect(&rec);

		rec.bottom = Bottom;
		rec.right = Right;

		pWnd->MoveWindow(rec);
		pChartCtrl->RefreshCtrl();
	}
}

void CGraphFull::OnCancel()
{
	CDialog::OnCancel();
}
