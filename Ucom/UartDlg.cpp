// UartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "UartDlg.h"
#include "afxdialogex.h"


// CUartDlg �Ի���

IMPLEMENT_DYNAMIC(CUartDlg, CDialog)

bool CUartDlg::uartPortIsOpen = false;
iUart CUartDlg::mUart;

CUartDlg::CUartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UART, pParent)
{

}

CUartDlg::~CUartDlg()
{
}

void CUartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUartDlg, CDialog)
	ON_CBN_DROPDOWN(IDC_CbUartPort, OnDropdownCbUartPort)
	ON_CBN_SELENDOK(IDC_CbUartPort, OnSelendokCbUartPort)
	ON_BN_CLICKED(IDC_BtnOpen, OnBtnOpen)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PicUartStatus, &CUartDlg::OnStnClickedPicuartstatus)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()



BOOL CUartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitPanel();
	//װ��ע�������
	LoadRegConfig();
	SwPanel(uartPortIsOpen);

	return TRUE; 
}


void CUartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	//������С����ָ�����ͼƬ����ʾ�����
	SwPanel(uartPortIsOpen);
}


void CUartDlg::InitPanel(void)
{
	CComboBox *pComBox;

	pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pComBox->ResetContent();
	//��ȡ�����б�
	mUart.GetComList(pComBox);
	//Ĭ��ѡ���һ��
	pComBox->SetCurSel(0);

	//��Ӳ�����
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartBaudrate);
	TCHAR BaudrateTable[][7] = { "460800", "230400", "194000", "115200", "57600", "56000", "38400"
		, "19200", "14400", "9600", "4800", "2400", "1200" };
	for (int i = 0; i < (sizeof(BaudrateTable) / sizeof(BaudrateTable[0])); i++)
		pComBox->InsertString(0, BaudrateTable[i]);

	//ѡ��115200ΪĬ��
	pComBox->SetCurSel(pComBox->FindString(-1, "115200"));

	//����λ
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartDatabit);
	pComBox->InsertString(0, "8");
	pComBox->InsertString(0, "7");
	pComBox->InsertString(0, "6");
	pComBox->InsertString(0, "5");

	pComBox->SetCurSel(3);//ѡ��8λΪĬ��

						  //ֹͣλ
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartStopbit);
	pComBox->InsertString(0, "2");
	pComBox->InsertString(0, "1.5");
	pComBox->InsertString(0, "1");

	pComBox->SetCurSel(0);//ѡ��115200λΪĬ��

	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartECC);
	pComBox->InsertString(0, "even");
	pComBox->InsertString(0, "odd ");
	pComBox->InsertString(0, "none");
	//ѡ����У��ΪĬ��
	pComBox->SetCurSel(0);
}

//�ӿռ�õ�������Ϣ�ַ���
DCB CUartDlg::GetUartConfigDCB(void)
{
	DCB configDCB;
	CString tmpStr, UartConfig;
	CComboBox *pCombox;

	//�趨���ڲ���
	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartBaudrate);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.BaudRate = _ttoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartECC);
	configDCB.Parity = pCombox->GetCurSel();

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartDatabit);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.ByteSize = atoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartStopbit);
	configDCB.StopBits = pCombox->GetCurSel();

	return configDCB;
}

void CUartDlg::ChangeBmpPic(int PicCtrlID, unsigned short nPicID)
{
	CBitmap bitmap;
	HBITMAP hBmp;
	CStatic *pStatic = (CStatic*)GetDlgItem(PicCtrlID);

	bitmap.LoadBitmap(nPicID);				// ��λͼIDB_BITMAP1���ص�bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // ��ȡbitmap����λͼ�ľ��

	pStatic->SetBitmap(hBmp);				// ����ͼƬ�ؼ�
}

void CUartDlg::SwPanel(bool choose)
{
	if (choose) {
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOn);
		SetDlgItemText(IDC_BtnOpen, _T("�رմ���"));
	}
	else
	{
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOff);
		SetDlgItemText(IDC_BtnOpen, _T("�򿪴���"));
	}

	choose = !choose;
	GetDlgItem(IDC_CbUartBaudrate)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartDatabit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartStopbit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartECC)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartPort)->EnableWindow(choose);
}

bool CUartDlg::IsUartPortAvailable(void)
{
	CString comInfo;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pCombox->GetLBText(pCombox->GetCurSel(), comInfo);

	if (comInfo.Left(3) == _T("COM"))
		return true;
	else
		return false;
}

void CUartDlg::OpenUart()
{
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	CString strtmp;

	if (uartPortIsOpen == false)
	{
		//���ռ�þ�ȡ��ռ��
		if (mUart.isConnected())
			mUart.ClosePort();

		if (IsUartPortAvailable())
		{
			pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
			mUart.ConfigUart(strtmp, GetUartConfigDCB());
		}
		else
		{
			AfxMessageBox(_T("������Ч,��ˢ��"));
			return;
		}

		if (!mUart.OpenCom())
		{
			AfxMessageBox(_T("�򿪴���ʧ��"));
			return;
		}

		//���������߳�
		hRxThread = AfxBeginThread(RxThreadFunc, mUart.GetThreadStartPara(), THREAD_PRIORITY_NORMAL);
		if (hRxThread == NULL) {
			TRACE("Rx Listenner Thread Created Failed");
			return;
		}

		uartPortIsOpen = true;
		//���ʹ���Ϣ
		//::SendMessage(::AfxGetMainWnd()->m_hWnd, WM_COMM_RX_MSG, 1, 0);
		//GetParent()->SendMessage(WM_COMM_RX_MSG, 1, 0);
		::PostMessage(HWND_BROADCAST, WM_MYONRECVMSG, W_UART_RECV, 0);
	}
	else if (uartPortIsOpen == true)
	{
		//����λ��־λ����ر�ʱ����մ���
		uartPortIsOpen = false;

		//���͹ر���Ϣ
		::PostMessage(HWND_BROADCAST, WM_MYONRECVMSG, W_UART_RECV, 0);

		TerminateThread(hRxThread, 0);
		//�رմ���
		if (mUart.isConnected())
			mUart.ClosePort();
	}
	// �л�ͼ�ꡢ��ֹ״̬����ť�ı�
	SwPanel(uartPortIsOpen);
}


//д��ע�������
void CUartDlg::WriteRegData(void)
{
	CComboBox *pCombox;
	CString strtmp;
	pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	//���ں�
	pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
	AfxGetApp()->WriteProfileString("Config", "ComName", strtmp);

	strtmp.Empty();
	strtmp.Format("%d-%d-%d-%d",
		((CComboBox*)GetDlgItem(IDC_CbUartBaudrate))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartDatabit))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartStopbit))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartECC))->GetCurSel());
	//��������
	AfxGetApp()->WriteProfileString("Config", "BDSE", strtmp);
}

void CUartDlg::GetRegData(CString &comName, CString &dcbConfig)
{
	comName = AfxGetApp()->GetProfileString("Config", "ComName", "NULL");
	dcbConfig = AfxGetApp()->GetProfileString("Config", "BDSE", "NULL");
	TRACE(comName + dcbConfig);
}

void CUartDlg::LoadRegConfig()
{
	CString comName, dcbConfig;
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbUartPort);

	GetRegData(comName, dcbConfig);
	if (comName != "NULL") {
		int sel = pComBox->FindString(-1, comName);
		//û�иô��ڣ����߲��Ǵ��ں�ʹ��Ĭ�ϣ���ʼ��ֵ��
		if (sel != CB_ERR && comName[0] == 'C')
			pComBox->SetCurSel(sel);

		int baudSel = 0, dataSel = 0, stopSel = 0, eccSel = 0;

		sscanf_s(dcbConfig, "%d-%d-%d-%d", &baudSel, &dataSel, &stopSel, &eccSel);
		((CComboBox*)GetDlgItem(IDC_CbUartBaudrate))->SetCurSel(baudSel);
		((CComboBox*)GetDlgItem(IDC_CbUartDatabit))->SetCurSel(dataSel);
		((CComboBox*)GetDlgItem(IDC_CbUartStopbit))->SetCurSel(stopSel);
		((CComboBox*)GetDlgItem(IDC_CbUartECC))->SetCurSel(eccSel);
	}
}

// CUartDlg ��Ϣ�������


//�˿��������ȿ���
void CUartDlg::OnDropdownCbUartPort()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}

//ѡȡ���һ��ˢ�¶���
void CUartDlg::OnSelendokCbUartPort()
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	int sel = pComBox->GetCurSel();
	int cnt = pComBox->GetCount();

	CString str;
	//ĩβˢ�´���
	if (sel == (cnt - 1))
	{
		//clear all
		pComBox->ResetContent();
		mUart.GetComList((CComboBox *)GetDlgItem(IDC_CbUartPort));

		//ѡȡ��һ��
		pComBox->SetCurSel(0);
	}
}


void CUartDlg::OnBtnOpen()
{
	OpenUart();
}



void CUartDlg::OnStnClickedPicuartstatus()
{
	OpenUart();
}
